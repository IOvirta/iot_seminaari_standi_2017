package com.iovirta.iot_kamera_sovellus;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.GregorianCalendar;

/**
 * Created by rytai on 15.3.2017.
 */

public class AppData {
    private static AppData mInstance= null;

    private boolean     videoList_json_loaded = false;
    private JSONObject  videoList_json;
    private ArrayList<VideoListEntry> videoListEntries;

    private String videoBaseUrl;
    private long LastTimeUpdateMillis = 0;

    public JSONObject getVideoList_json() {
        return videoList_json;
    }
    public void setVideoList_json(JSONObject videoList_json) {
        this.videoList_json = videoList_json;
    }
    public boolean isVideoList_json_loaded() {
        return videoList_json_loaded;
    }
    public void setVideoList_json_loaded(boolean videoList_json_loaded) {
        this.videoList_json_loaded = videoList_json_loaded;
    }

    public void setVideoListEntries(ArrayList<VideoListEntry> videoListEntries){
        this.videoListEntries = videoListEntries;
    }
    public ArrayList<VideoListEntry> getVideoListEntries(){
        return this.videoListEntries;
    }

    protected AppData(){}

    public static synchronized AppData getInstance(){
        if(null == mInstance){
            mInstance = new AppData();
        }
        return mInstance;
    }

    public String getVideoBaseUrl() {
        return videoBaseUrl;
    }

    public void setVideoBaseUrl(String videoBaseUrl) {
        this.videoBaseUrl = videoBaseUrl;
    }

    public long getLastTimeUpdateMillis() {
        return LastTimeUpdateMillis;
    }

    public void setLastTimeUpdateMillis(long lastTimeUpdateMillis) {
        LastTimeUpdateMillis = lastTimeUpdateMillis;
    }
    //MyProperties.getInstance().someValueIWantToKeep
}