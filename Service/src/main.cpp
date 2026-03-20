#include <iostream>
#include <chrono>
#include <thread>

#include <utils.h>

#include <serial/serial.h>

int main()
{
    std::string port = "";
    const unsigned long baud = 115200;
    serial::Serial mySerial;

    while (true)
    {
        try
        {
            if (!mySerial.isOpen())
            {
                port = utils::autoDetectPort();
                if (port == "")
                {
                    std::cout << "[Status][main()] Device not found. Retrying in 3s..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    continue;
                }
            }

            std::cout << "[Status][main()] Connecting to " << "port: " << port << " at speed: " << baud << " ..." << std::endl;

            mySerial.setPort(port);
            mySerial.setBaudrate(baud);
            auto timeout = serial::Timeout::simpleTimeout(1000);
            mySerial.setTimeout(timeout);
            mySerial.open();

            // waiting due to controller DTR reset
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "[Status][main()] Connected successfully!" << std::endl;
        }
        catch (serial::IOException& e) 
        {
            std::cerr << "[Error][main()] Connection failed! " << std::endl;
            std::cerr << "[Error][main()] Detalis: " << e.what() << std::endl;
            std::cerr << "[Error][main()] Retrying in 2s..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }
        catch (std::invalid_argument& e)
        {
            std::cerr << "[Error][main()] Invalid arguments (wrong baudrate or port)!" << std::endl;
            break;
        }
    
        while (mySerial.isOpen())
        {
            try
            {
                if (mySerial.available())
                {
                    std::string data = mySerial.readline();

                    if (data.length() >= 1)
                    {
                        std::cout << "[Status][main()] Received data: " << data << std::endl;

                        if (data.find("HOIN_TIME") != std::string::npos)
                        {
                            // return local unix timestamp of the host
                        }
                        else if (data.find("HOIN_LOCALIP") != std::string::npos)
                        {
                            // return local ip of the host
                        }
                    }
                }
            }
            catch (serial::SerialException& e)
            {
                std::cerr << "[Error][main()] Device disconnected!" << std::endl;
                break;
            }
            catch (serial::IOException& e)
            {
                std::cerr << "[Error][main()] IO Error during read!" << std::endl;
                break;
            }
            catch (const std::exception& e) 
            {
                std::cerr << "[Error][main()] Exception: " << e.what() << std::endl;
            }
            catch (...) 
            {
                std::cerr << "[Error][main()] Unknown error!" << std::endl;
            }
        }
        
    }
    

    return 0;
}