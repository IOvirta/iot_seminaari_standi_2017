package com.iovirta.iot_kamera_sovellus;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.util.LogPrinter;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.json.JSONObject;

import java.io.InputStream;

public class MainActivity extends AppCompatActivity {


    public final static String EXTRA_MESSAGE = "com.iovirta.iot_kamera_sovellus.MESSAGE";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent intent = getIntent();
    }

    /** Called when the user clicks the Send button */
    public void openMediaView(View view) {
        Intent intent = new Intent(this, mediaView.class);
        startActivity(intent);

    }
    /** Called when the user clicks the Send button */
    public void openJsonView(View view) {
        Intent intent = new Intent(this, jsontest.class);
        startActivity(intent);

    }


}
