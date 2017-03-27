package com.iovirta.iot_kamera_sovellus;

import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.util.Log;
import android.widget.MediaController;
import android.widget.VideoView;

import java.io.FileNotFoundException;
import java.net.URL;

public class mediaView extends AppCompatActivity {


    private String urlStream;
    private VideoView myVideoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media_view);


        myVideoView = (VideoView)this.findViewById(R.id.myVideoView);
        MediaController mc = new MediaController(this);
        myVideoView.setMediaController(mc);
        //urlStream = "http://bjuutro.dy.fi:8080/HLS/live.m3u8";
        //urlStream = "http://www.html5videoplayer.net/videos/toystory.mp4";
        //urlStream = "http://avaruuskeskittyma.cf:7755/hls/live.m3u8";
        urlStream = "http://10.10.206.228:7755/hls/live.m3u8";
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //myVideoView.setVideoPath(Uri.parse(urlStream));
                myVideoView.setVideoURI(Uri.parse(urlStream));
                myVideoView.start();
            }
        });
    }
}
