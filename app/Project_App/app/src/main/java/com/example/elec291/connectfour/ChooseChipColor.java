package com.example.elec291.connectfour;

import android.content.Intent;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.widget.Button;
import android.os.Bundle;
import android.widget.TextView;
import android.view.ViewGroup;
import android.view.View;
import android.widget.RelativeLayout;

import java.util.ArrayList;

/**
 * Created by rohini on 23/03/16.
 */
public class ChooseChipColor extends AppCompatActivity implements View.OnClickListener{
    Button buttonRed, buttonBlue, buttonGreen, buttonMagenta, buttonYellow, buttonCyan, buttonNext;
    TextView colorText2, textView3, textView4, textView5, textView6, textView7, textView8;
    ArrayList<Button> allColorbuttons = new ArrayList<Button>();
    int clicks = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_choose_chip_color);


        //Shrink Size of pop-up down
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height * 0.8));

        //initialization
        buttonRed = (Button) findViewById(R.id.buttonRed);
        buttonBlue = (Button) findViewById(R.id.buttonBlue);
        buttonGreen = (Button) findViewById(R.id.buttonGreen);
        buttonMagenta = (Button) findViewById(R.id.buttonMagenta);
        buttonYellow = (Button) findViewById(R.id.buttonYellow);
        buttonCyan = (Button) findViewById(R.id.buttonCyan);
        buttonNext = (Button) findViewById(R.id.buttonNext);

        buttonRed.setOnClickListener(this);
        buttonBlue.setOnClickListener(this);
        buttonGreen.setOnClickListener(this);
        buttonMagenta.setOnClickListener(this);
        buttonYellow.setOnClickListener(this);
        buttonCyan.setOnClickListener(this);
        buttonNext.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        if(clicks == 0) {
            switch ((v.getId())) {
                case R.id.buttonRed:
                    clicks = 1;
                    colorButtonClick(buttonRed);
                    break;
                case R.id.buttonBlue:
                    clicks = 1;
                    colorButtonClick(buttonBlue);
                    break;
                case R.id.buttonGreen:
                    clicks = 1;
                    colorButtonClick(buttonGreen);
                    break;
                case R.id.buttonCyan:
                    clicks = 1;
                    colorButtonClick(buttonCyan);
                    break;
                case R.id.buttonYellow:
                    clicks = 1;
                    colorButtonClick(buttonYellow);
                    break;
                case R.id.buttonMagenta:
                    clicks = 1;
                    colorButtonClick(buttonMagenta);
                    break;
            }
        }

        else if(clicks == 1){
            switch((v.getId())){
                case R.id.buttonRed:
                    clicks = 2;
                    onClickTwo(buttonRed);
                    break;
                case R.id.buttonBlue:
                    clicks = 2;
                    onClickTwo(buttonBlue);
                    break;
                case R.id.buttonGreen:
                    clicks = 2;
                    onClickTwo(buttonGreen);
                    break;
                case R.id.buttonCyan:
                    clicks = 2;
                    onClickTwo(buttonCyan);
                    break;
                case R.id.buttonYellow:
                    clicks = 2;
                    onClickTwo(buttonYellow);
                    break;
                case R.id.buttonMagenta:
                    clicks = 2;
                    onClickTwo(buttonMagenta);
                    break;
            }

        }

        else if(clicks == 2){
            switch ((v.getId())){
                case R.id.buttonNext:
                    finish();
                    startActivity(new Intent("com.example.elec291.connect4.ConnectViaWiFi"));
                    break;
            }
        }
    }

    private void onClickTwo(Button pressedButton){
        switch(pressedButton.getId()){
            case R.id.buttonMagenta:
                textView6.setVisibility(View.VISIBLE);
                ( (TextView) findViewById(R.id.textView6)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonYellow:
                ( (TextView) findViewById(R.id.textView7)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonRed:
                ( (TextView) findViewById(R.id.textView3)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonCyan:
                ( (TextView) findViewById(R.id.textView4)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonGreen:
                ( (TextView) findViewById(R.id.textView8)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonBlue:
                ( (TextView) findViewById(R.id.textView5)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
        }
    }

    private void colorButtonClick(Button pressedButton){

        switch (pressedButton.getId()){
            case R.id.buttonRed:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView3 = (TextView) findViewById(R.id.textView3);
                textView3.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonBlue:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView5 = (TextView) findViewById(R.id.textView5);
                textView5.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonGreen:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView8 = (TextView) findViewById(R.id.textView8);
                textView8.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonMagenta:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView6 = (TextView) findViewById(R.id.textView6);
                textView6.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonCyan:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView4 = (TextView) findViewById(R.id.textView4);
                textView4.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonYellow:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView7 = (TextView) findViewById(R.id.textView7);
                textView7.setVisibility(View.VISIBLE);
                break;
        }
        //Change the text to tell user to choose a different color
        ((TextView) findViewById(R.id.textView2)).setText("Second Color");
    }

    private void NextClickToConnectScreen(){
        startActivity(new Intent("com.example.elec291.connect4.ConnectViaWiFi"));
    }

//    private void secondColorButtonClick(Button pressedButton){
//        allColorbuttons.add(buttonBlue);
//        allColorbuttons.add(buttonCyan);
//        allColorbuttons.add(buttonGreen);
//        allColorbuttons.add(buttonMagenta);
//        allColorbuttons.add(buttonRed);
//        allColorbuttons.add(buttonYellow);
//
//        for(int i = 0; i < allColorbuttons.size(); i++){
//            if(allColorbuttons.get(i) != pressedButton){
//                allColorbuttons.get(i).setOnClickListener(this);
//            }
//        }
//    }
}