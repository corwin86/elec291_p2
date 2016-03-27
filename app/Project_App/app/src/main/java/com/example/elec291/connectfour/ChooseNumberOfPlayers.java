package com.example.elec291.connectfour;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;

/**
 * Created by rohini on 26/03/16.
 */
public class ChooseNumberOfPlayers extends Activity implements View.OnClickListener{

    Button buttonSinglePlayer, buttonMultiPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_choose_number_of_players);
        //athis.requestWindowFeature(Window.FEATURE_NO_TITLE);


        //initialization
        buttonSinglePlayer = (Button) findViewById(R.id.buttonSinglePlayer);
        buttonSinglePlayer.setOnClickListener(this);

        buttonMultiPlayer = (Button) findViewById(R.id.buttonMultiPlayer);
        buttonMultiPlayer.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
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
            startActivity(new Intent(ChooseNumberOfPlayers.this, DisplayColorsActivity.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }
        else{
            startActivity(new Intent(ChooseNumberOfPlayers.this, ChooseChipColor.class));
            //startActivity(new Intent(ChoosePlayerMode.this, ConnectViaWifi.class));
        }

        //startActivity(new Intent("com.example.elec291.connect4.DisplayColorsActivity"));

    }
}
