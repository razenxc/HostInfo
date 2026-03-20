#include <utils.h>

#include <iostream>
#include <chrono>
#include <thread>

#include <serial/serial.h>

namespace utils
{
    std::string autoDetectPort()
    {
        std::cout << "[Status][autoDetectPort()] Scanning ports..." << std::endl;
        
        std::vector<serial::PortInfo> devices = serial::list_ports();
        std::vector<serial::PortInfo>::iterator it = devices.begin();

        while (it != devices.end())
        {
            serial::PortInfo device = *it++;
            std::string port = device.port;

            std::cout << "[Status][autoDetectPort()] Checking " << port  << "... " << std::endl;

            try
            {
                serial::Serial mySerial(port, 115200, serial::Timeout::simpleTimeout(1000));

                if (mySerial.isOpen())
                {
                    // Waiting due to controller DTR reset
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                    mySerial.flushInput();
                    mySerial.write("HOIN_HEY\n");

                    std::string data = mySerial.read(50);

                    if (data.find("HOIN_OK") != std::string::npos)
                    {
                        std::cout << "[Status][autoDetectPort()] Found!" << std::endl;
                        return port;
                    }
                    else
                    {
                        std::cout << "[Status][autoDetectPort()] Port not found. Trying the next one." << std::endl;
                    }

                    mySerial.close();
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }

        return "";
    }
}