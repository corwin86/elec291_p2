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

/**
 * Created by rohini on 24/03/16.
 */
public class ConnectViaWifi extends AppCompatActivity implements View.OnClickListener {

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
        buttonConnect.setOnClickListener(this);
//        buttonConnect.setOnClickListener(new View.OnClickListener() {
//
//            @Override
//            public void onClick(View v) {
//                //startActivity(new Intent("com.example.elec291.connect4.GameBoard"));
//                //try{
//                    //String lineRead = WifiConnection.POST(testURL);
//                try {
//                    POST(testURL);
//                    connectMessage.setText(str);
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//
//
////               / }
////                catch (IOException e) {
////                    e.printStackTrace();
////                    connectMessage.setText("Did not send message");
////                }
//                SystemClock.sleep(3000);
//                startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));
//
//            }
//        });

    }

    //@Override
    public void onClick(View view){
        URL connect_url = null;

            String stringUrl = "http://192.168.43.82/";
            ConnectivityManager connmgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo = connmgr.getActiveNetworkInfo();
            if(networkInfo != null && networkInfo.isConnected()){
                System.out.println("Network available");
                new DownloadWebpageTask().execute(urlToConnection);
            }
            else{
                System.out.println("no network connection available");
            }
//
//            connect_url = new URL(urlToConnection);
//            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
//            connection.setDoOutput(true);
//            connection.setChunkedStreamingMode(0);
//            OutputStream out = new BufferedOutputStream(connection.getOutputStream());
//            writeStream(out);


        startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));
    }

    private class DownloadWebpageTask extends AsyncTask<String, Void, String>{

        @Override
        protected String doInBackground(String...urls){

            //params comes from the execute() call
            try{
                //return downloadUrl(urls[0]);
                Upload(urls[0]);
                return "worked";
            } catch(IOException e){
                return "unable to retrieve webpage";
            }
        }

        @Override
        protected void onPostExecute(String result){
            System.out.println(result);
        }

    }

    private String downloadUrl(String myurl) throws IOException{
        InputStream is = null;
        int len = 500;

        try{
            URL connect_url = new URL(myurl);
            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
            System.out.println("Checkpoint1");
            connection.setReadTimeout(10000);
            connection.setConnectTimeout(15000);
            connection.setRequestMethod("GET");
            connection.setDoInput(true);
            System.out.println("Checkpoint2");

            System.out.println("Checkpoint3");
            connection.connect();
            System.out.println("Checkpoint 4");
            System.out.println(connection.getResponseCode());
            //int response = connection.getResponseCode();
            //System.out.println(response);
            System.out.println("Checkpoint 5");
            is = connection.getInputStream();
            System.out.println("Checkpoint 6");

            String contectAsString = readIt(is, len);
            System.out.println(contectAsString);
            return contectAsString;
        } finally{
            if(is != null){
                is.close();
            }
        }
    }

    private void Upload(String myurl) throws IOException {
        OutputStream os = null;
        int len = 500;
        try{
            URL connect_url = new URL(myurl);
            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
            connection.setReadTimeout(10000);
            connection.setConnectTimeout(15000);
            connection.setRequestMethod("POST");
            connection.setDoOutput(true);
            connection.connect();
            int response = connection.getResponseCode();
            System.out.println(response);
            writeIt(os);

        }
        finally {
            if(os != null){
                os.close();
            }
        }
    }

    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException{
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }

    public void writeIt(OutputStream stream) throws IOException{
        String output = "First post";
        stream.write(output.getBytes());
        stream.flush();

    }


//    public void POST(String url) throws IOException {
//        URL connect_url = new URL(url);
//        HttpURLConnection httpConnection = (HttpURLConnection) connect_url.openConnection();
//
//        try {
//            InputStream in = new BufferedInputStream(httpConnection.getInputStream());
//            str = readStream(in);
//            //return str;
//        } finally {
//            httpConnection.disconnect();
//        }
//    }
//
//    public static String readStream(InputStream in) throws IOException {
//        StringBuilder sb = new StringBuilder();
//        BufferedReader r = new BufferedReader(new InputStreamReader(in),1000);
//        for (String line = r.readLine(); line != null; line =r.readLine()){
//            sb.append(line);
//        }
//        in.close();
//        return sb.toString();
//    }
//
//    public void writeStream(OutputStream out) throws IOException {
//        String written = "Android Connection";
//        out.write(written.getBytes());
//
//    }
}
