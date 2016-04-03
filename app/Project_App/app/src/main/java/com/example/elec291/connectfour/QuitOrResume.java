package com.example.elec291.connectfour;

import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 03/04/16.
 */
public class QuitOrResume extends AppCompatActivity implements View.OnClickListener {

    Button Quit, Resume;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_quit_or_resume);

        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height *0.8));

        Quit = (Button) findViewById(R.id.buttonquit);
        Resume = (Button) findViewById(R.id.buttonResume);
        Quit.setOnClickListener(this);
        Resume.setOnClickListener(this);

    }

    @Override
    public void onClick(View v){
        switch ((v.getId())){
            case R.id.buttonquit:
                finish();
                SystemClock.sleep(500);
                startActivity(new Intent(QuitOrResume.this, MainActivity.class));
                break;
            case R.id.buttonResume:
                finish();
                break;
        }

    }

}
