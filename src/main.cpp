#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <cstdio>

#include "inotify++.hpp"

#include <asio.hpp>

#include "conf/conf.hpp"
#include "video/camera.hpp"
#include "network/fcm_server.hpp"

std::string current_time(const std::string &format);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " conf.ini" << std::endl;
        return 0;
    }

    iovirta_iot::conf::Configuration conf(argv[1]);

    iovirta_iot::video::Camera camera(conf.get<int>("camera0", "id"),
        conf.get<int>("camera0", "width"), conf.get<int>("camera0", "height"));


    iovirta_iot::network::FCMServer fcm_server; // TODO oisko tää jonku muun niminen oikeesti

    const char *ffmpeg_command_fmt = "ffmpeg -f rawvideo -pixel_format bgr24 -video_size %dx%d -i pipe:0 -codec:v %s -pix_fmt %s -vprofile baseline -an -f flv %s";
    int ffmpeg_command_len = std::snprintf(nullptr, 0, ffmpeg_command_fmt, camera.width(), camera.height(),
        conf.get<std::string>("video", "codec").c_str(), conf.get<std::string>("video", "pix_fmt").c_str(), conf.get<std::string>("video", "output").c_str());

    std::vector<char> ffmpeg_command_buf(ffmpeg_command_len + 1);
    std::snprintf(&ffmpeg_command_buf[0], ffmpeg_command_buf.size(), ffmpeg_command_fmt, camera.width(), camera.height(),
        conf.get<std::string>("video", "codec").c_str(), conf.get<std::string>("video", "pix_fmt").c_str(), conf.get<std::string>("video", "output").c_str());

    //std::FILE *ffmpeg = popen(std::string(ffmpeg_command_buf.begin(), ffmpeg_command_buf.end()).c_str(), "w");
    //if (ffmpeg == nullptr)
    //{
    //    std::cerr << "ffmpeg popen" << std::endl;
    //    return 1;
    //}             vconcatille -i kansio_missä_videot ja -o kansio_mihin_videot TODO <<<--------
    std::FILE *vconcat = popen(conf.get<std::string>("locations", "vconcat").c_str(), "w");
    if (vconcat == nullptr)
    {
        std::cerr << ".....todo" << std::endl;
    }

    std::vector<std::string> client_tokens;

    {
        /* luetaan puhelinten tokenit */
        std::ifstream is(conf.get<std::string>("locations", "tokens"));
        if (!is.is_open())
        {
            std::cerr << "avainlistan '" <<  conf.get<std::string>("locations", "tokens") << "' avaus epäonnistui, onko conffissa oikein?" << std::endl;
            return 1;
        }
        std::copy(std::istream_iterator<std::string>(is), std::istream_iterator<std::string>(), std::back_inserter(client_tokens));
    }

    for (auto &token : client_tokens)
        fcm_server.add_client(token);


    /* inotifylla saadaan tietää jos tiedostoissa/kansioissa tapahtuu muutoksia */
    inotify::Inotify inotify;
    inotify.add_watch(conf.get<std::string>("locations", "tokens"), IN_MODIFY);
    inotify.add_watch(conf.get<std::string>("locations", "hls"), IN_CREATE);

    cv::namedWindow("video");
    bool movement = false;

    auto movement_stop_time = std::chrono::system_clock::now();
    bool asddsa = false; // joku kuvaavampi nimi

    std::string latest_video = "";
    std::string latest_full_video = "";


    while (true)
    //for (int i = 0; i < conf.get<int>("test", "frameja"); i++)
    {
        if (!camera.capture_frame())
        { /* ei saatu kuvaa */ break;}


        if (camera.check_motion())
        {
            if (!movement)
            {
                /* liike alkoi */
                movement = true;
                std::cout << "alkoi" << std::endl;

                std::string vconcat_cmd = "add " + conf.get<std::string>("locations", "hls") + latest_full_video;
                fwrite(vconcat_cmd.c_str(), 1, vconcat_cmd.length(), vconcat);
                std::cout << vconcat_cmd << std::endl;
            }
        }
        else
        {
            if (movement)
            {
                std::cout << "ajanotto alotettu" << std::endl;
                asddsa = true;
                movement = false;
                movement_stop_time = std::chrono::system_clock::now();
            }
        }

        if (!movement && asddsa)
        {
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - movement_stop_time).count();

            if (seconds >= 5)
            {
                asddsa = false;
                fwrite("concat", 1, 6, vconcat);
                std::cout << "loppui" << std::endl;
            }
        }

        std::string timestamp = current_time("%d.%m.%y %T");
        auto timestamp_size = cv::getTextSize(timestamp, CV_FONT_HERSHEY_SIMPLEX, 1, 3, nullptr);
        cv::putText(camera.frame_, timestamp, {camera.width() - timestamp_size.width - 10, camera.height() - timestamp_size.height},
            CV_FONT_HERSHEY_SIMPLEX,1, cv::Scalar::all(255));


        cv::imshow("video", camera.frame_);
        cv::waitKey(1);
        //fwrite(camera.frame_.data, sizeof(unsigned char), frame_size, ffmpeg);

        /* kysytään inotifyltä onko uusia muutoksia tarkailtavissa tiedostoissa */
        if (inotify.poll_events())
        {
            while (auto event = inotify.get_event())
            {
                if (event.mask & IN_MODIFY)
                {
                    /* uuden puhelimen tokeni tullut, otetaan se muistiin */
                    std::vector<std::string> tokens_tmp;
                    std::ifstream is(conf.get<std::string>("locations", "tokens"));

                    std::copy(std::istream_iterator<std::string>(is), std::istream_iterator<std::string>(), std::back_inserter(tokens_tmp));

                    /* lisätään kaikki uudet tokenit fcm lähettäjälle (fcm_server)*/
                    for (auto it = tokens_tmp.begin() + client_tokens.size(); it != tokens_tmp.end(); ++it)
                    {
                        client_tokens.push_back(*it);
                        fcm_server.add_client(*it);
                    }
                }
                if (event.mask & IN_CREATE)
                {
                    /* uus videon pätkä tullut, otetaan sen nimi ylös ja merkataan
                       edellinen video uusimmaks (koska tää uus video ei oo vielä kokonainen)
                     */
                     latest_full_video = latest_video;
                     latest_video = event.name;
                }
            }
        }
    }
    //fwrite("quit", 1, 4, vconcat);
    //pclose(vconcat);
    //pclose(ffmpeg);

    /*iovirta_iot::network::FCMServer fcmserver;

	try
	{
		//io_service.run();
		fcmserver.send();
		std::cout << "Script completed\n";
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cout << "error\n";
	}*/

    return 0;
}

std::string current_time(const std::string &format)
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream time_ss;
    time_ss << std::put_time(std::localtime(&now_time_t), format.c_str());
    return time_ss.str();
}
