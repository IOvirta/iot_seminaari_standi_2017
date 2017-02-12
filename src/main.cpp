#include <iostream>
#include <atomic>
//#include <thread>
#include <chrono>


#include <opencv2/highgui/highgui.hpp> // tulee poistumaan

#include <asio.hpp>

#include "conf/conf.hpp"
#include "video/camera.hpp"
#include "video/motion.hpp"
#include "video/encoder.hpp"
#include "network/video_streamer.hpp"
#include "network/fcm_server.hpp"


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " conf.ini" << std::endl;
        return 0;
    }

    iovirta_iot::conf::Configuration conf(argv[1]);

    iovirta_iot::video::Camera camera;
    if (!iovirta_iot::video::open_camera(conf.get<int>("camera0", "id"), camera))
    {
        return 1;
    }

    /* kameran asetukset */
    camera.set_width(conf.get<int>("camera0", "width"));
    camera.set_height(conf.get<int>("camera0", "height"));

    iovirta_iot::video::Encoder encoder(conf.get<std::string>("video", "codec"), camera.get_width(), camera.get_height());

    /* tai joku muu --v, esim std::bitset */
    std::atomic<std::uint8_t> flags;

    iovirta_iot::network::VideoStreamer streamer/*(...)*/;
    /*std::thread ....*/
    iovirta_iot::network::FCMServer fcm_server/*()
    std::thread ....., std::ref(flags));*/;

    // v--------- aikanaan poistuu
    if (conf.get<bool>("test", "nayta"))
        cv::namedWindow("frame");

    bool movement = false;
    std::chrono::time_point<std::chrono::steady_clock> movement_stop_time;
    // while (flags & run)
    for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        if (!iovirta_iot::video::capture_frame(camera))
        { /* ei saatu kuvaa */ }

        if (iovirta_iot::video::check_motion(camera))
        {
            /*  jos edellisellä framella ei ollut liikettä lähetetään puhelimelle
                ilmotus ja avataan tiedosto tallentamista vartern*/
            if (!movement)
            {
                //fcm_server.send(/* kenelle lähetetään, mitä läheteään*/);
                encoder.open_file("" /*kenties xxx_yymmdd_hhmmss.pääte?*/);
                movement = true;
            }
        }
        else
        {
            /*jos viime framella oli liikettä pistetään kello pyöriin
            muuten katotaan onko kello pöyriny x aikaa ja jos on
            niin pistetää tiedosto kii

            eli siis tallennetaan tiedostoon vielä x aikaa liikkeen loppumisen jälkee*/
            if (movement)
                movement_stop_time = std::chrono::steady_clock::now();
            else
            {
                auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - movement_stop_time);
                if (time_elapsed > std::chrono::seconds(2/* confista hakee oikeesti*/))
                    encoder.close_file();
            }
        }

        //if (puhelin pyytänyt kuvaa || kamerassa liikettä)
        if (flags & 1/*ei oikeesti 1 mut joku muu arvo, esim laheta_kuvaa*/ || movement)
        {
            auto encoded_frame = encoder.encode_frame(/*camera.frame*/);

            //if (pyydetty puhelimelta, eli siis sama ku tuo flags & )
            //    streamer.send_frame(camera.frame);
        }



        // v---- aikanaan poistuu
        if (conf.get<bool>("test", "nayta"))
            cv::imshow("frame", camera.frame);

        cv::waitKey(1);
    }

    /* threadien pysytykset yms*/

    iovirta_iot::network::FCMServer fcmserver = iovirta_iot::network::FCMServer();

	try
	{
		//io_service.run();
		fcmserver.send();
		std::cout << "Script completed\n";
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cout << "error\n";
	}

    return 0;
}
