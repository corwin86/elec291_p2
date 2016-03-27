package com.example.elec291.connectfour;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 24/03/16.
 */
public class ConnectViaWifi extends Activity {

    Button buttonConnect;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_connect_to_wifi);

        //initialization
        buttonConnect = (Button) findViewById(R.id.buttonConnect);
        buttonConnect.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //startActivity(new Intent("com.example.elec291.connect4.GameBoard"));
                startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));

            }
        });

    }
}
