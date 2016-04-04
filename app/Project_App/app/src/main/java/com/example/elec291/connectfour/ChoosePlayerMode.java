package com.example.elec291.connectfour;

import android.app.Activity;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.content.Intent;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;

import java.util.concurrent.ExecutionException;


public class ChoosePlayerMode extends Activity implements View.OnClickListener {

    Button buttonSinglePlayer, buttonMultiPlayer, buttonAI;
    static String PlayerModeSelected;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_choose_player_mode);
        setTitle("Choose the Number of Players");

//        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
//        setSupportActionBar(toolbar);
//
//
//        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        //Initialization
        buttonSinglePlayer = (Button) findViewById(R.id.buttonSinglePlayer);
        buttonSinglePlayer.setOnClickListener(this);

        buttonMultiPlayer = (Button) findViewById(R.id.buttonMultiPlayer);
        buttonMultiPlayer.setOnClickListener(this);

        buttonAI = (Button) findViewById(R.id.ButtonAIPlayer);
        buttonAI.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        switch ((v.getId())){
            case R.id.buttonSinglePlayer:
                PlayerModeSelected = "single";
                onPlayerClick(buttonSinglePlayer);
                break;
            case R.id.buttonMultiPlayer:
                PlayerModeSelected = "multi";
                onPlayerClick(buttonMultiPlayer);
                break;
            case R.id.ButtonAIPlayer:
                PlayerModeSelected = "ai";
                onPlayerClick(buttonAI);
                break;
        }

    }

    public void onPlayerClick(Button pressedButton){
//        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
//        try{
//            String toPost = stringToBePosted();
//            String responseString = wifiConnection.doPOST(toPost, MainActivity.urlToConnection);
//            System.out.println(responseString);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        } catch (ExecutionException e) {
//            e.printStackTrace();
//        }
        if(pressedButton == buttonSinglePlayer){
            startActivity(new Intent(ChoosePlayerMode.this, DisplayColorsActivity.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }
        else if(pressedButton == buttonMultiPlayer){
            startActivity(new Intent(ChoosePlayerMode.this, ChooseChipColor.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }
        else if (pressedButton == buttonAI){
            startActivity(new Intent(ChoosePlayerMode.this, ChooseAIColors.class));
        }
    }

    public String stringToBePosted(){
        return "mode:\t" + PlayerModeSelected;
    }
}
