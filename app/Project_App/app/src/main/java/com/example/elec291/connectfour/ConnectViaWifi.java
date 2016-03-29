package com.example.elec291.connectfour;

import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by rohini on 24/03/16.
 */
public class ConnectViaWifi extends AppCompatActivity {

    Button buttonConnect;
    TextView connectMessage;
    String str;
    String urlToConnection = "http://192.168.43.82/";

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_connect_to_wifi);


//        try{
////            Socket s = new Socket("192.168.43.82", 80);
////
////            //outgoing stream redirect to socket
////            OutputStream out = s.getOutputStream();
////
////            PrintWriter output = new PrintWriter(out);
////            output.println("Hello Android!");
////            BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
////
////            st = input.readLine();
////            s.close();
//        } catch (UnknownHostException e) {
//            e.printStackTrace();
//        } catch (IOException e) {
//            e.printStackTrace();
//        }


        //initialization
        connectMessage = (TextView) findViewById(R.id.textView10);
        buttonConnect = (Button) findViewById(R.id.buttonConnect);
        buttonConnect.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                //startActivity(new Intent("com.example.elec291.connect4.GameBoard"));
                try{

                    String lineRead = WifiConnection.POST(urlToConnection);
                    connectMessage.setText(WifiConnection.st);

                } catch (IOException e) {
                    e.printStackTrace();
                    connectMessage.setText("Did not send message");
                }
                SystemClock.sleep(3000);
                startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));

            }
        });

    }
}
