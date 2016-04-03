package com.example.elec291.connectfour;

import android.content.Intent;
import android.graphics.Color;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.widget.Button;
import android.os.Bundle;
import android.widget.TextView;
import android.view.ViewGroup;
import android.view.View;
import android.widget.RelativeLayout;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.concurrent.ExecutionException;

/**
 * Created by rohini on 23/03/16.
 */
public class DisplayColorsActivity extends AppCompatActivity implements View.OnClickListener{
    Button buttonRed, buttonBlue, buttonGreen, buttonMagenta, buttonYellow, buttonCyan, buttonNext;
    TextView colorText2, textView3, textView4, textView5, textView6, textView7, textView8, textView12,
            textView13, textView14, textView15, textView16, textView17;
    ArrayList<Button> allColorbuttons = new ArrayList<Button>();
    int clicks = 0;

    String Player1Color;
    String Player2Color;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_choose_chip_color);


        //Shrink Size of pop-up down
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        setTitle("Choose a chip color");

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
                    Player1Color = "red";
                    clicks = 1;
                    colorButtonClick(buttonRed);
                    break;
                case R.id.buttonBlue:
                    Player1Color = "blue";
                    clicks = 1;
                    colorButtonClick(buttonBlue);
                    break;
                case R.id.buttonGreen:
                    Player1Color = "green";
                    clicks = 1;
                    colorButtonClick(buttonGreen);
                    break;
                case R.id.buttonCyan:
                    Player1Color = "cyan";
                    clicks = 1;
                    colorButtonClick(buttonCyan);
                    break;
                case R.id.buttonYellow:
                    Player1Color = "yellow";
                    clicks = 1;
                    colorButtonClick(buttonYellow);
                    break;
                case R.id.buttonMagenta:
                    Player1Color = "magenta";
                    clicks = 1;
                    colorButtonClick(buttonMagenta);
                    break;
            }
        }

        else if(clicks == 1){
            switch((v.getId())){
                case R.id.buttonRed:
                    Player2Color = "red";
                    clicks = 2;
                    onClickTwo(buttonRed);
                    break;
                case R.id.buttonBlue:
                    Player2Color = "blue";
                    clicks = 2;
                    onClickTwo(buttonBlue);
                    break;
                case R.id.buttonGreen:
                    Player2Color = "green";
                    clicks = 2;
                    onClickTwo(buttonGreen);
                    break;
                case R.id.buttonCyan:
                    Player2Color = "cyan";
                    clicks = 2;
                    onClickTwo(buttonCyan);
                    break;
                case R.id.buttonYellow:
                    Player2Color = "yellow";
                    clicks = 2;
                    onClickTwo(buttonYellow);
                    break;
                case R.id.buttonMagenta:
                    Player2Color = "magenta";
                    clicks = 2;
                    onClickTwo(buttonMagenta);
                    break;
            }

        }

        else if(clicks == 2){
            switch ((v.getId())){
                case R.id.buttonNext:
                    finish();
                    SystemClock.sleep(500);
                    startActivity(new Intent(DisplayColorsActivity.this, GameBoard.class));
                    //startActivity(new Intent(DisplayColorsActivity.this, GameInstructions.class));
                    //startActivity(new Intent("com.example.elec291.connect4"));
                    break;
            }
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
        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        try{
            String player1Post = player1colorPost();
            String responseString = wifiConnection.doPOST(player1Post, MainActivity.urlToConnection);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        //Change the text to tell user to choose a different color
        //((TextView) findViewById(R.id.textView2)).setText("Second Color");
        ((TextView) findViewById(R.id.textView2)).setText("Choose Chip Color for CPU");
       // MainActivity.player1Color = pressedButton.getBackground();
        setTitle("Choose a Chip Color for the CPU");

    }

    private void onClickTwo(Button pressedButton) {
        switch (pressedButton.getId()) {
            case R.id.buttonMagenta:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView15 = (TextView) findViewById(R.id.textView15);
                textView15.setVisibility(View.VISIBLE);
                // ((TextView) findViewById(R.id.textView6)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonYellow:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView16 = (TextView) findViewById(R.id.textView16);
                //((TextView) findViewById(R.id.textView7)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonRed:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView12 = (TextView) findViewById(R.id.textView12);
                textView12.setVisibility(View.VISIBLE);
                // ((TextView) findViewById(R.id.textView3)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonCyan:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView13 = (TextView) findViewById(R.id.textView13);
                textView13.setVisibility(View.VISIBLE);
                //((TextView) findViewById(R.id.textView4)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonGreen:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                //pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView17 = (TextView) findViewById(R.id.textView17);
                textView17.setVisibility(View.VISIBLE);
                //((TextView) findViewById(R.id.textView8)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
            case R.id.buttonBlue:
                pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                textView14 = (TextView) findViewById(R.id.textView14);
                textView14.setVisibility(View.VISIBLE);
                //pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
                //((TextView) findViewById(R.id.textView5)).setText("Second Player");
                buttonNext.setVisibility(View.VISIBLE);
                break;
        }
        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        try{
            String player2Post = player2colorPost();
            String responseString = wifiConnection.doPOST(player2Post, MainActivity.urlToConnection);
            System.out.println(responseString);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        //MainActivity.player2Color = pressedButton.getBackground();

    }

    private String player1colorPost(){
        return "player1colour\t" + Player1Color;

    }

    private String player2colorPost(){
        return "player2colour\t" + Player2Color;

    }
}