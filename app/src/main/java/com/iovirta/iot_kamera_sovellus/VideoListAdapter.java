package com.iovirta.iot_kamera_sovellus;


import java.util.ArrayList;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;

public class VideoListAdapter extends ArrayAdapter<VideoListEntry> {
    public VideoListAdapter(Context context, ArrayList<VideoListEntry> list_of_video_entrys) {
        super(context, 0, list_of_video_entrys);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // Get the data item for this position
        VideoListEntry video_entry = getItem(position);
        // Check if an existing view is being reused, otherwise inflate the view
        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.video_list_entry, parent, false);
        }
        // Lookup view for data population
        TextView caption = (TextView) convertView.findViewById(R.id.vle_caption);
        Button openvideo_Button = (Button) convertView.findViewById(R.id.vle_button);
        // Populate the data into the template view using the data object
        caption.setText(video_entry.date);
        final String url = video_entry.url;
        openvideo_Button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                //Combine url and pass it inside intent to mediaview class
                Context context = v.getContext();
                Intent i = new Intent(context, mediaView_from_url.class);
                i.putExtra("videourl", url);
                context.startActivity(i);
            }
        });
        // Return the completed view to render on screen
        return convertView;
    }
}