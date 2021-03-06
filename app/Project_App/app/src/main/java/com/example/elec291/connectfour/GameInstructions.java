package com.example.elec291.connectfour;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.concurrent.ExecutionException;

/**
 * Created by rohini on 24/03/16.
 */
public class GameInstructions extends Activity implements View.OnClickListener{
    Button buttonDone;
    //boolean start_pressed = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_game_instructions);
        setTitle("How to Play");

        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height * 0.8));

        buttonDone = (Button) findViewById(R.id.buttonDone);
        buttonDone.setOnClickListener(this);


//        if(!GameBoard.start_has_been_pressed){
//            buttonDone.setText("START");
//        }
//        else{
//            buttonDone.setText("DONE");
//        }
    }

    @Override
    public void onClick(View v){

        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        try{
            String start = "st";
            String responseString = wifiConnection.doPOST(start, MainActivity.urlToConnection);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        GameBoard.start_has_been_pressed = true;
        finish();

    }
}



