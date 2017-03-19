package com.iovirta.iot_kamera_sovellus;

import java.util.ArrayList;

/**
 * Created by rytai on 16.3.2017.
 */

public class VideoListEntry {

    public String url;
    public String date;

    public VideoListEntry(String url, String date) {
        this.url = url;
        this.date = date;
    }

    @Override
    public String toString(){
        String str = "Url:"+this.url+",date:"+this.date;
        return str;
    }


}
