package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ListView videoList = findViewById(R.id.video_list);
        String[] videoTitles = {"작은 계곡", "밤의 분수대", "길 위의 폭포"};
        Integer[] videoIds = {R.raw.creek_rock, R.raw.fountain_night, R.raw.waterfall_street};

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, videoTitles);
        videoList.setAdapter(adapter);

        videoList.setOnItemClickListener((parent, view, position, id) -> {
            Intent intent = new Intent(MainActivity.this, VideoActivity.class);
            intent.putExtra("videoId", videoIds[position]);
            startActivity(intent);
        });
    }
}
