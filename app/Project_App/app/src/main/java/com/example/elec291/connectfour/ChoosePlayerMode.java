package com.example.elec291.connectfour;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;
import java.util.concurrent.ExecutionException;


public class ChoosePlayerMode extends Activity implements View.OnClickListener {

    Button buttonSinglePlayer, buttonMultiPlayer, buttonAI;
    static String PlayerModeSelected;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.content_choose_player_mode);

        //Change title of action bar
        setTitle("Choose the Number of Players");


        //Initialization of the different player mode buttons
        buttonSinglePlayer = (Button) findViewById(R.id.buttonSinglePlayer);
        buttonSinglePlayer.setOnClickListener(this);

        buttonMultiPlayer = (Button) findViewById(R.id.buttonMultiPlayer);
        buttonMultiPlayer.setOnClickListener(this);

        buttonAI = (Button) findViewById(R.id.ButtonAIPlayer);
        buttonAI.setOnClickListener(this);

    }

    //depending on which player mode was selected, change the string that is going
    //to be posted to the server
    @Override
    public void onClick(View v) {
        switch ((v.getId())) {
            case R.id.buttonSinglePlayer:
                v.getBackground().setAlpha(100);
                PlayerModeSelected = "ms";
                onPlayerClick(buttonSinglePlayer);
                break;
            case R.id.buttonMultiPlayer:
                v.getBackground().setAlpha(100);
                PlayerModeSelected = "mm";
                onPlayerClick(buttonMultiPlayer);
                break;
            case R.id.ButtonAIPlayer:
                v.getBackground().setAlpha(100);
                PlayerModeSelected = "ma";
                onPlayerClick(buttonAI);
                break;
        }

    }

    //Post the player mode selected to the server
    public void onPlayerClick(Button pressedButton) {
        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        try {
            String responseString = wifiConnection.doPOST(PlayerModeSelected, MainActivity.urlToConnection);
            System.out.println(responseString);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }

        startActivity(new Intent(ChoosePlayerMode.this, ChooseChipColor.class));

    }
}