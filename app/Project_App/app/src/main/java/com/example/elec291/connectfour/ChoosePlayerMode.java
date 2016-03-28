package com.example.elec291.connectfour;

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


public class ChoosePlayerMode extends AppCompatActivity implements View.OnClickListener {

    Button buttonSinglePlayer, buttonMultiPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_player_mode);
        setTitle("Choose the Number of Players");

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);


        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        //Initialization
        buttonSinglePlayer = (Button) findViewById(R.id.buttonSinglePlayer);
        buttonSinglePlayer.setOnClickListener(this);

        buttonMultiPlayer = (Button) findViewById(R.id.buttonMultiPlayer);
        buttonMultiPlayer.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        switch ((v.getId())){
            case R.id.buttonSinglePlayer:
                onPlayerClick(buttonSinglePlayer);
                break;
            case R.id.buttonMultiPlayer:
                onPlayerClick(buttonMultiPlayer);
                break;
        }

    }

    public void onPlayerClick(Button pressedButton){
        if(pressedButton == buttonSinglePlayer){
            startActivity(new Intent(ChoosePlayerMode.this, DisplayColorsActivity.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }
        else{
            startActivity(new Intent(ChoosePlayerMode.this, ChooseChipColor.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }

        //startActivity(new Intent("com.example.elec291.connect4.DisplayColorsActivity"));

    }
}
