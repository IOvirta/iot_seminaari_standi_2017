package com.iovirta.iot_kamera_sovellus;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;

/**
 * Created by rytai on 15.3.2017.
 */

class JsonDownloader extends AsyncTask<Void,Void,Integer>
{
    private Handler ui_handler_;

    public JsonDownloader(Handler ui_handler){
        ui_handler_ = ui_handler;
    }

    protected void onPreExecute() {
        //display progress dialog.

    }
    protected Integer doInBackground(Void... param) {
        try {
            Log.d("json", "entering json download");
            URL url = new URL("http://avaruuskeskittyma.cf:7755/listvideos.php");
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            con.setDoOutput(true);
            String responseMsg = con.getResponseMessage();
            int response = con.getResponseCode();

            StringBuilder result = new StringBuilder();

            InputStream in = new BufferedInputStream(con.getInputStream());

            BufferedReader reader = new BufferedReader(new InputStreamReader(in));

            String line;
            while ((line = reader.readLine()) != null) {
                result.append(line);
            }

            Log.d("json","httpreq: responsecode: "+response);
            Log.d("json","httpreq: response: "+responseMsg);
            Log.d("json",result.toString());

            try {

                JSONObject obj = new JSONObject(result.toString());
                AppData.getInstance().setVideoList_json(obj);
                AppData.getInstance().setVideoList_json_loaded(true);

                //Log.d("json", AppData.getInstance().getVideoList_json().toString());
                Log.d("json", "Json_downloaded: "+obj.toString());

                String baseurl = obj.getString("urlbase");
                AppData.getInstance().setVideoBaseUrl(baseurl);

                ArrayList<VideoListEntry> videoListEntries = new ArrayList<VideoListEntry>();

                //Videolist from JSON
                JSONArray videosArr = obj.getJSONArray("videos");
                for (int i = 0; i < videosArr.length(); i++) {
                    JSONObject obj_ = videosArr.getJSONObject(i);
                    //Whole path to video
                    String url_ =   "http://"+baseurl + obj_.getString("url"); //huom "/"-merkki
                    String date_ =  obj_.getString("date");
                    VideoListEntry newVideo = new VideoListEntry(url_, date_);
                    videoListEntries.add(newVideo);
                }

                AppData.getInstance().setVideoListEntries(videoListEntries);

            } catch (JSONException e) {
                e.printStackTrace();
                return 0;
            }


        } catch (MalformedURLException e) {
            e.printStackTrace();
            return 0;
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("json", "maybe something wrong with url or the server side?");
            return 0;
        }
        return 1;
    }

    //1 - onnistui
    //2 - epäonnistui
    protected void onPostExecute(Integer result) {
        // dismiss progress dialog and update ui

        Message msg=new Message();
        msg.arg1 = result;
        ui_handler_.sendMessage(msg);
    }
}