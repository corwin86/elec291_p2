package com.example.elec291.connectfour;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.support.v7.app.AppCompatActivity;
import android.app.Activity;
import android.util.DisplayMetrics;
import android.view.Window;
import android.widget.Button;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.TextView;
import android.view.View;
import android.widget.Toast;
import android.view.View.OnClickListener;

public class MainActivity extends AppCompatActivity{

    Button playButton;
    static Drawable player1Color;
    static Drawable player2Color;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.activity_main);


        //Initialization
        playButton = (Button) findViewById(R.id.playButton);
        playButton.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v) {
                //startActivity(new Intent(MainActivity.this, Pop.class));
                startActivity(new Intent(MainActivity.this, ChoosePlayerMode.class));
            }
        });
//        playButton.setOnClickListener(new View.OnClickListener(){
//
//
//
//            @Override
//            public void onClick(View v){
//                startActivity(new Intent(MainActivity.this, Pop.class));
//            }
//
//        });
        //playButton.setBackgroundColor(Color.parseColor("#FFF1D5D5"));
        //playButton.setColorFilter( 0xffff0000, Mode.MULTIPLY )    }

    }
    private void playButtonClick(){
        startActivity(new Intent("com.example.elec291.connect4.ChoosePlayerMode"));
    }



//    @Override
//    public void onClick(View v) {
//        switch ((v.getId())){
//            case R.id.playButton:
//                playButtonClick();
//                break;
//
//        }
//
//    }
}
 