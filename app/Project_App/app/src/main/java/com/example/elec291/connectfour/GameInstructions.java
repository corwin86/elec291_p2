package com.example.elec291.connectfour;

import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;
import java.util.concurrent.ExecutionException;

/**
 * Created by rohini on 24/03/16.
 */

public class GameInstructions extends Activity implements View.OnClickListener{
    Button buttonDone;

    @Override
    public void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.content_game_instructions);

        //Change title of action bar
        setTitle("How to Play");

        //Change the size of the view to create a pop-up window
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height * 0.8));

        //initializing the Done Button
        buttonDone = (Button) findViewById(R.id.buttonDone);
        buttonDone.setOnClickListener(this);

    }

    //When done is pressed, close activity and go to game board. Send start signal to the server
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



