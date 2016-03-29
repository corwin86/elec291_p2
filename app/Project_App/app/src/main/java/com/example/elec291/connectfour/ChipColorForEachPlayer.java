package com.example.elec291.connectfour;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 28/03/16.
 */
public class ChipColorForEachPlayer extends AppCompatActivity implements View.OnClickListener {

    Button player1, player2, next;
    Button buttonPressedForColor;


    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_chip_color_for_each_player);

        //Initialization
        player1 = (Button) findViewById(R.id.buttonPlayer1);
        player2 = (Button) findViewById(R.id.buttonPlayer2);
        next = (Button) findViewById(R.id.buttonNextToConnect);

        player1.setOnClickListener(this);
        player2.setOnClickListener(this);
        next.setOnClickListener(this);

        if(ChoosePlayerMode.selectedPlayerMode == ChoosePlayerMode.buttonSinglePlayer){
            player1.setText("Player 1");
            player2.setText("CPU");
        }

        if (ChoosePlayerMode.selectedPlayerMode == ChoosePlayerMode.buttonMultiPlayer){
            player1.setText("Player 1");
            player2.setText("Player 2");
        }
    }

    @Override
    public void onClick(View v){
        switch (v.getId()){
            case R.id.buttonPlayer1:
                buttonPressedForColor = player1;
                startActivity(new Intent(ChipColorForEachPlayer.this, ChooseColorForChip.class));
                break;
            case R.id.buttonPlayer2:
                startActivity(new Intent(ChipColorForEachPlayer.this, ChooseColorForChip.class));
                break;
            }


        }
    }

