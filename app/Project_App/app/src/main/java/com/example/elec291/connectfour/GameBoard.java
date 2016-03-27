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

    Button instructions;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_game_board);

        //initialization of buttons
        instructions = (Button) findViewById(R.id.buttonInstructions);
        instructions.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
        switch(v.getId()){
            case R.id.buttonInstructions:
                startActivity(new Intent(GameBoard.this, GameInstructions.class));
        }
    }
}
