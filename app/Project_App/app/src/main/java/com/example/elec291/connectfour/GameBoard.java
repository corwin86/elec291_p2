package com.example.elec291.connectfour;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;
import java.lang.Integer;

import java.util.concurrent.ExecutionException;

/**
 * Created by rohini on 25/03/16.
 */
public class GameBoard extends Activity implements View.OnClickListener{

    Button instructions, Column1, Column2, Column3, Column4, Column5, Column6, Column7, Column8;
    TextView currentPlayer;
    String move;
    static boolean start_has_been_pressed = false;
    int player = 1; //1 = player 1. 2 = player2
    String lastResponse = "3";

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_game_board);

        //initialization of buttons
        instructions = (Button) findViewById(R.id.buttonInstructions);
        instructions.setOnClickListener(this);
        currentPlayer = (TextView) findViewById(R.id.textView11);

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

        startActivity(new Intent(GameBoard.this, GameInstructions.class));
    }

    @Override
    public void onClick(View v){
        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        switch(v.getId()){
            case R.id.buttonInstructions:
                startActivity(new Intent(GameBoard.this, GameInstructions.class));
                break;
            case R.id.column1:
                move = "0";
                break;
            case R.id.column2:
                move = "1";
                break;
            case R.id.column3:
                move = "2";
                break;
            case R.id.column4:
                move = "3";
                break;
            case R.id.column5:
                move = "4";
                break;
            case R.id.column6:
                move = "5";
                break;
            case R.id.column7:
                move = "6";
                break;
            case R.id.column8:
                move = "7";
                break;
        }
        try {
            String playerResponse = wifiConnection.doPOST(move, MainActivity.urlToConnection).trim();

            if(!lastResponse.equals(playerResponse)) {
                player = player == 1 ? 2 : 1;
                if(player == 1) {
                    currentPlayer.setText("Player 1: Make a Move");
                } else {
                    currentPlayer.setText("Player 2: Make a Move");
                }
            }

            System.out.println(lastResponse);
            lastResponse = playerResponse;
            System.out.println(lastResponse);

//            if(playerResponse.equals("1")){
//                player = 1;
//                currentPlayer.setText("Player 1: Make a Move");
//            }
//            else if (playerResponse.equals("2")){
//                player = 2;
//                currentPlayer.setText("Player 2: Make a Move");
//            }
//            System.out.println(playerResponse);
        } catch (ExecutionException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

//
}
//@Override
//    public boolean onKeyDown(int keyCode, KeyEvent event){
//        if(keyCode == KeyEvent.KEYCODE_BACK){
//            startActivity(new Intent(GameBoard.this, QuitOrResume.class));
//            return true;
//        }
//        return super.onKeyDown(keyCode, event);
//    }