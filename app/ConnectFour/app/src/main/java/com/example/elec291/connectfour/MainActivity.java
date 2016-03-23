package com.example.elec291.connectfour;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.app.Activity;
import android.util.DisplayMetrics;
import android.widget.Button;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;
import android.view.View;
import android.widget.Toast;
import android.view.View.OnClickListener;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button playButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.activity_main);


        //Initialization
        playButton = (Button) findViewById(R.id.playButton);
        playButton.setOnClickListener(this);

    }


    private void playButtonClick(){
        startActivity(new Intent("com.example.elec291.connect4.ChoosePlayerMode"));
    }


    @Override
    public void onClick(View v) {
        switch ((v.getId())){
            case R.id.playButton:
                playButtonClick();
                break;

        }

    }
}
 