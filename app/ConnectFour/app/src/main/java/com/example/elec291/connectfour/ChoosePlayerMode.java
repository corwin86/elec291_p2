package com.example.elec291.connectfour;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

public class ChoosePlayerMode extends AppCompatActivity implements View.OnClickListener {

    Button buttonSinglePlayer, buttonMultiPlayer;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_choose_player_mode);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });
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
        startActivity(new Intent("com.example.elec291.connect4.DisplayColorsActivity"));

    }
}
