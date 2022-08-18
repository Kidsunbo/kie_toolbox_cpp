#include <context/context.hpp>
#include <iostream>

int now(){
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void run_as_daemon(){
    //run with specified concurrent
    kie::context ctx1(10);
    ctx1.get_one();
    ctx1.run_as_daemon();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ctx1.stop();
    std::cout<<"[KIE] "<<now()<<" ctx1 run_as_daemon stopped"<<std::endl;

    //run with implicit context limit and specified concurrency limit
    kie::context ctx2;
    ctx2.get_all();
    ctx2.run_as_daemon(40);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ctx2.stop();
    std::cout<<"[KIE] "<<now()<<" ctx2 run_as_daemon stopped"<<std::endl;

    //run with explicit constructor from io_context
    boost::asio::io_context ctx;
    kie::context ctx3(ctx);
    ctx3.run_as_daemon();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ctx3.stop();
    std::cout<<"[KIE] "<<now()<<" ctx3 run_as_daemon stopped"<<std::endl;
}

void run(){
    //run with specified concurrent
    kie::context ctx1(10);
    ctx1.get_one();
    std::thread([&]{
        std::this_thread::sleep_for(std::chrono::seconds(3));
        ctx1.stop();
    }).detach();
    ctx1.run();
    std::cout<<"[KIE] "<<now()<<" ctx1 run stopped"<<std::endl;

    //run with implicit context limit and insane specified concurrency limit
    kie::context ctx2;
    ctx2.get_all();
    std::thread([&]{
        std::this_thread::sleep_for(std::chrono::seconds(3));
        ctx2.stop();
    }).detach();
    ctx2.run(40);
    std::cout<<"[KIE] "<<now()<<" ctx2 run stopped"<<std::endl;

    //run with explicit context that wrap io_context
    boost::asio::io_context ctx;
    kie::context ctx3(ctx);
    ctx3.get_all();
    std::thread([&]{
        std::this_thread::sleep_for(std::chrono::seconds(3));
        ctx3.stop();
    }).detach();
    ctx3.run(40);
    std::cout<<"[KIE] "<<now()<<" ctx3 run stopped"<<std::endl;
}


void all_test(){
    std::cout<<"[KIE] "<<now()<<" start test"<<std::endl;
    run();
    std::cout<<"[KIE] "<<now()<<" finish run and start run as daemon"<<std::endl;
    run_as_daemon();
    std::cout<<"[KIE] "<<now()<<" finish run as daemon"<<std::endl;
}


int main(){
    all_test();
    return 0;
}