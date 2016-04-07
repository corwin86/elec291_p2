package com.example.elec291.connectfour;

import android.content.Intent;
import android.app.Activity;
import android.widget.Button;
import android.os.Bundle;
import android.view.View;


public class MainActivity extends Activity {

    Button playButton;

    //IP address created by the arduino wifi shield
    static String urlToConnection = "http://192.168.43.82/";


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.activity_main);

        //Change title of action bar
        setTitle("  ");


        //Initialization of the play button
        playButton = (Button) findViewById(R.id.playButton);
        playButton.setOnClickListener(new View.OnClickListener() {


            //when the play button is pressed, start a new activity
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, ChoosePlayerMode.class));
            }
        });


    }
}
