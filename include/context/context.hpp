/**
 * @file context.hpp
 * @author Kie
 *
 * Define the kie::context for every use case. It's wrapper that works for many
 * situation. It follows per context per thread model.
 */

#ifndef KIE_TOOLBOX_CONTEXT_CONTEXT_HPP
#define KIE_TOOLBOX_CONTEXT_CONTEXT_HPP

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <tuple>

/**
 * @brief context is in kie namespace
 * 
 * 
 */
namespace kie
{

  /** @brief A wrapper for list of boost::asio::io_context.
   *
   * Use this context is just for easy handling when per thread per model is needed.
   * This class is non-movable and non-copyable.
   *
   * To create on context, use following syntax:
   * @code
   * //use ten threads to execute.
   * kie::context ctx{10};
   *
   * //do some bussiness...
   *
   * ctx.run();
   *
   * @endcode
   *
   */
  class context
  {
    using context_ptr = std::unique_ptr<boost::asio::io_context, void(*)(boost::asio::io_context*)>;
    std::vector<context_ptr> all_ctx;
    std::vector<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> guards;
    mutable std::size_t current_idx;

  private:

    static void default_deleter(boost::asio::io_context* ptr){
      delete ptr;
    }

    static void noop_deleter(boost::asio::io_context*){}

  public:
    /**
     * @brief The constructor which use CPU number as its concurrency limit. It may throw exception when memory allocation fails.
     *
     * @param size How many context should be used.
     */
    context() : context(std::thread::hardware_concurrency()) {}

    /**
     * @brief The explicit constructor for context. It may throw exception when memory allocation fails.
     *
     *
     * @param size How many context should be used.
     */
    explicit context(std::size_t size) : current_idx(0)
    {
      for (std::size_t i = 0; i < size; i++)
      {
        all_ctx.emplace_back(context_ptr(new boost::asio::io_context(1), default_deleter));
        guards.emplace_back(all_ctx[i]->get_executor());
      }
    }

    /**
     * @brief The explicit constructor for context from io_context. It may throw exception when memory allocation fails.
     *
     * The caller should keep io_context live longer than this.
     *
     * @param ctx The `io_context` that will be wrapped.
     */
    explicit context(boost::asio::io_context& ctx) : current_idx(0)
    {
      all_ctx.emplace_back(context_ptr(&ctx, noop_deleter));
      guards.emplace_back(ctx.get_executor());
    }

    /**
     * @brief Copy constructor is deleted.
     *
     * Copying is not allowed.
     */
    context(const context &) = delete;

    /**
     * @brief Move constructor is deleted.
     *
     * Moving is not allowed.
     */
    context(context &&) = delete;

    /**
     * @brief Get one context from pool.
     *
     * It use round-robin algorithm to fetch one context.
     */
    boost::asio::io_context &get_one()
    {
      const auto &ctx = all_ctx[current_idx++];
      current_idx %= all_ctx.size();
      return *ctx;
    }

    /**
     * @brief Get one context from pool by index.
     *
     * If someone wants use specific context by index. This is the function should be used.
     *
     * @param idx The index of context wanted. It should be less than the size of the pool.
     *
     * @return A pair of context and existence. If existence is false, the context is the first one, and should not be used.
     */
    std::tuple<boost::asio::io_context &, bool> get(std::size_t idx)
    {
      if (idx >= all_ctx.size())
      {
        return {*all_ctx[0], false};
      }
      return {*all_ctx[idx], true};
    }

    /**
     * @brief Get all the io_context in the pool.
     *
     * All the io_context will be returned.
     *
     *
     * @return The reference to the underlining pool.
     */
    const std::vector<context_ptr> &get_all() const
    {
      return all_ctx;
    }

    /**
     * @brief Run the context as daemon. It does not block current thread.
     *
     * @param concurrency_hint. The hint to context how many threads should be used to run. Default to -1, which means per context per thread.
     */
    void run_as_daemon(std::size_t concurrency_hint = 0)
    {
      concurrency_hint = std::max(all_ctx.size(), concurrency_hint);
      for (std::size_t i = 0; i < concurrency_hint; i++)
      {
        std::thread([=, this]
                    { all_ctx[i % all_ctx.size()]->run(); })
            .detach();
      }
    }

    /**
     * @brief Run the context and block current thread.
     *
     * 
     * This start the context runtime and serving all the async operation.
     *
     * @param concurrency_hint. The hint to context how many threads should be used to run. Default to -1, which means per context per thread. 
     */
    void run(std::size_t concurrency_hint = 0)
    {
      concurrency_hint = std::max(all_ctx.size(), concurrency_hint);
      std::vector<std::thread> threads;
      for (std::size_t i = 0; i < concurrency_hint; i++)
      {
        threads.emplace_back([=, this]
                             { all_ctx[i % all_ctx.size()]->run(); });
      }

      for (auto &t : threads)
      {
        t.join();
      }
    }

    /**
     * @brief Stop the context runtime.
     *
     * This will stop all the io_context and continue to work gracefully.
     */
    void stop()
    {
      for (auto &item : all_ctx)
      {
        item->stop();
      }
    }
  };

} // namespace kie

#endif