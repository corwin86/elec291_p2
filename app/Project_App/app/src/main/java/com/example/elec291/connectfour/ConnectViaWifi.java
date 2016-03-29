package com.example.elec291.connectfour;

import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;
import java.net.UnknownHostException;

/**
 * Created by rohini on 24/03/16.
 */
public class ConnectViaWifi extends AppCompatActivity {

    Button buttonConnect;
    TextView connectMessage;
    static String  str;
    String urlToConnection = "http://192.168.43.82/";
    String testURL = "http://www.android.com/";

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
                //try{
                    //String lineRead = WifiConnection.POST(testURL);
                    connectMessage.setText(str);

//               / }
//                catch (IOException e) {
//                    e.printStackTrace();
//                    connectMessage.setText("Did not send message");
//                }
                SystemClock.sleep(3000);
                startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));

            }
        });

    }

    public void POST(String url) throws IOException {
        URL connect_url = new URL(url);
        HttpURLConnection httpConnection = (HttpURLConnection) connect_url.openConnection();

        try {
            InputStream in = new BufferedInputStream(httpConnection.getInputStream());
            str = readStream(in);
            //return str;
        } finally {
            httpConnection.disconnect();
        }
    }

    public static String readStream(InputStream in) throws IOException {
        StringBuilder sb = new StringBuilder();
        BufferedReader r = new BufferedReader(new InputStreamReader(in),1000);
        for (String line = r.readLine(); line != null; line =r.readLine()){
            sb.append(line);
        }
        in.close();
        return sb.toString();
    }
}
