package com.example.elec291.connectfour;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 25/03/16.
 */
public class GameBoard extends AppCompatActivity implements View.OnClickListener{

    Button instructions, Column1, Column2, Column3, Column4, Column5, Column6, Column7, Column8;
    String move;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_game_board);

        //initialization of buttons
        instructions = (Button) findViewById(R.id.buttonInstructions);
        instructions.setOnClickListener(this);

        Column1 = (Button) findViewById(R.id.column1);
        Column2 = (Button) findViewById(R.id.column2);
        Column3 = (Button) findViewById(R.id.column3);
        Column4 = (Button) findViewById(R.id.column4);
        Column5 = (Button) findViewById(R.id.column5);
        Column6 = (Button) findViewById(R.id.column6);
        Column7 = (Button) findViewById(R.id.column7);
        Column8 = (Button) findViewById(R.id.column8);

        Column1.setOnClickListener(this);
        Column2.setOnClickListener(this);
        Column3.setOnClickListener(this);
        Column4.setOnClickListener(this);
        Column5.setOnClickListener(this);
        Column6.setOnClickListener(this);
        Column7.setOnClickListener(this);
        Column8.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
        switch(v.getId()){
            case R.id.buttonInstructions:
                startActivity(new Intent(GameBoard.this, GameInstructions.class));
                break;
            case R.id.column1:
                break;
            case R.id.column2:
                break;
            case R.id.column3:
                break;
            case R.id.column4:
                break;
            case R.id.column5:
                break;
            case R.id.column6:
                break;
            case R.id.column7:
                break;
            case R.id.column8:
                break;
        }
    }
}
