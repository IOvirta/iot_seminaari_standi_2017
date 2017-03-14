package com.iovirta.iot_kamera_sovellus;

import android.os.AsyncTask;
import android.os.Debug;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

public class jsontest extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jsontest);

        new MyDownloadTask().execute();
    }

    class MyDownloadTask extends AsyncTask<Void,Void,Void>
    {


        protected void onPreExecute() {
            //display progress dialog.

        }
        protected Void doInBackground(Void... params) {
            URL url = null;
            try {
                url = new URL("http://avaruuskeskittyma.cf:7755/listvideos.php");
                HttpURLConnection con = (HttpURLConnection) url.openConnection();
                con.setDoOutput(true);
                String responseMsg = con.getResponseMessage();
                Log.d("httptest", responseMsg);
                int response = con.getResponseCode();

                StringBuilder result = new StringBuilder();

                InputStream in = new BufferedInputStream(con.getInputStream());

                BufferedReader reader = new BufferedReader(new InputStreamReader(in));

                String line;
                while ((line = reader.readLine()) != null) {
                    result.append(line);
                }
                Log.d("httptest", result.toString());

                try {

                    JSONObject obj = new JSONObject(result.toString());

                    Log.d("httptest", obj.toString());
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            } catch (MalformedURLException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }


        protected void onPostExecute(Void result) {
            // dismiss progress dialog and update ui

        }
    }


}