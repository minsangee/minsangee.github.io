package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.net.Uri;
import android.os.Bundle;
import android.widget.VideoView;

public class VideoActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);

        VideoView videoView = findViewById(R.id.video_view);
        int videoId = getIntent().getIntExtra("videoId", -1);

        if (videoId != -1) {
            String uriPath = "android.resource://" + getPackageName() + "/" + videoId;
            Uri uri = Uri.parse(uriPath);
            videoView.setVideoURI(uri);
            videoView.start();
        }
        videoView.setOnClickListener(v -> {

            finish();
        });
    }
}