
package com.example.elec291.connectfour;

import android.content.AsyncQueryHandler;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.Socket;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutionException;

/**
 * Created by rohini on 24/03/16.
 */
public class ConnectViaWifi extends AppCompatActivity implements View.OnClickListener {

    Button buttonConnect;
    TextView connectMessage;


    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_connect_to_wifi);

        //initialization
        connectMessage = (TextView) findViewById(R.id.textView10);
        buttonConnect = (Button) findViewById(R.id.buttonConnect);
        buttonConnect.setOnClickListener(this);
    }

    //@Override
    public void onClick(View view){
        String modeAndColors = createStringBody();
        WifiConnection wifiConnection = new WifiConnection(getApplicationContext());
        try {
            String responseString = wifiConnection.doPOST(modeAndColors, MainActivity.urlToConnection);
            System.out.println(responseString);
        } catch (ExecutionException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));
    }

    public String createStringBody(){
        String concatenated;
        concatenated = "Mode:\t" + ChoosePlayerMode.PlayerModeSelected + "\n" + "Player1 Color: \t" + MainActivity.Player1Color +  "\n" + "Player2 Color: \t" + MainActivity.Player2Color;
        return concatenated;
    }

}