package com.iovirta.iot_kamera_sovellus;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Debug;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.util.LogPrinter;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {


    public final static String EXTRA_MESSAGE = "com.iovirta.iot_kamera_sovellus.MESSAGE";

    //Videolistaus
    public Handler mainUIHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    @Override
    protected void onResume(){
        super.onResume();

        mainUIHandler = new Handler() {

            @Override
            public void handleMessage(Message msg) {
                // TODO Auto-generated method stub
                Log.d("json", "handlemessage (json loaded) success: "+ Integer.toString(msg.arg1));
                if (msg.arg1 == 1) {

                    populateUsersList();
                }
            }

        };

        new JsonDownloader(mainUIHandler).execute();

    }


    /** Called when the user clicks the Send button */
    public void openMediaView(View view) {
        Intent intent = new Intent(this, mediaView.class);
        startActivity(intent);

    }
    /** Called when the user clicks the Send button */
    public void openCreditsView(View view) {
        Intent intent = new Intent(this, credits.class);
        startActivity(intent);

    }

    public void openMediaViewWithUrl(View view, String url) {
        //Get the base of url
        String urlBase = AppData.getInstance().getVideoBaseUrl();
        //Combine url and pass it inside intent to mediaview class
        Intent i = new Intent(this, mediaView_from_url.class);
        i.putExtra("videourl", urlBase+url);
        startActivity(i);
    }

    //return Success/failure
    private boolean populateUsersList() {

        if(!AppData.getInstance().isVideoList_json_loaded()){
            TextView videoRecordinstext = (TextView)findViewById(R.id.videorecordingsheader);
            videoRecordinstext.setText(getResources().getText(R.string.video_recordings_header_novideos));
            return false;
        }

        ArrayList<VideoListEntry> videoListArray = AppData.getInstance().getVideoListEntries();

        // Get the data source
        // Create the adapter to convert the array to views
        VideoListAdapter adapter = new VideoListAdapter(this, videoListArray);
        // Attach the adapter to a ListView
        ListView listView = (ListView) findViewById(R.id.videolist_listview);
        listView.setAdapter(adapter);
        return true;
    }

}
