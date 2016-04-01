
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

//        ConnectivityManager connmgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
//        NetworkInfo networkInfo = connmgr.getActiveNetworkInfo();
//        if(networkInfo != null && networkInfo.isConnected()){
//            System.out.println("Network available");
//            new DownloadWebpageTask().execute(urlToConnection);
//        }
//        else{
//            System.out.println("no network connection available");
//        }
//
//            connect_url = new URL(urlToConnection);
//            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
//            connection.setDoOutput(true);
//            connection.setChunkedStreamingMode(0);
//            OutputStream out = new BufferedOutputStream(connection.getOutputStream());
//            writeStream(out);


        startActivity(new Intent(ConnectViaWifi.this, GameBoard.class));
    }

//    private class DownloadWebpageTask extends AsyncTask<String, Void, String>{
//
//        @Override
//        protected String doInBackground(String...urls){
//
//            //params comes from the execute() call
//            try{
//                //return downloadUrl(urls[0]);
//                return Upload(urls[0]);
//            } catch(IOException e){
//                return "unable to retrieve webpage";
//            }
//        }
//
//        @Override
//        protected void onPostExecute(String result){
//            System.out.println(result);
//        }
//
//    }
//
//    private String downloadUrl(String myurl) throws IOException{
//        InputStream is = null;
//        int len = 500;
//
//        try{
//            URL connect_url = connect_url = new URL(myurl);
//            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
//            System.out.println("Checkpoint1");
//            connection.setReadTimeout(10000);
//            connection.setConnectTimeout(15000);
//            connection.setRequestMethod("GET");
//            connection.setDoInput(true);
//            System.out.println("Checkpoint2");
//
//            System.out.println("Checkpoint3");
//            connection.connect();
//            System.out.println("Checkpoint 4");
//            System.out.println(connection.getResponseCode());
//            //int response = connection.getResponseCode();
//            //System.out.println(response);
//            System.out.println("Checkpoint 5");
//            is = connection.getInputStream();
//            System.out.println("Checkpoint 6");
//
//            String contectAsString = readIt(is, len);
//            System.out.println(contectAsString);
//            return contectAsString;
//        } finally{
//            if(is != null){
//                is.close();
//            }
//        }
//    }
//
//    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException{
//        Reader reader = null;
//        reader = new InputStreamReader(stream, "UTF-8");
//        char[] buffer = new char[len];
//        reader.read(buffer);
//        return new String(buffer);
//    }
//
//    private String Upload(String myurl) throws IOException {
//        //OutputStream os = null;
//        DataOutputStream os = null;
//        BufferedReader reader=  null;
//        int len = 500;
//        try{
//            String text = "";
//            URL connect_url = new URL(myurl);
//            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
//            System.out.println("Checkpoint1");
//            connection.setReadTimeout(10000);
//            connection.setConnectTimeout(15000);
//            connection.setRequestMethod("POST");
//            connection.setRequestProperty("Content-Type", "text/plain");
//            connection.setDoOutput(true);
//            connection.setDoInput(true);
//            System.out.println("Checkpoint2");
//            connection.connect();
//            System.out.println("Checkpoint3");
//            //int response = connection.getResponseCode();
//            //System.out.println(connection.getResponseCode());
//            System.out.println("Checkpoint4");
//            //System.out.println(response);
//            //os = connection.getOutputStream();
//            System.out.println("Checkpoint getOutputStream");
//            //os = connection.getOutputStream();
//            os = new DataOutputStream(connection.getOutputStream());
//            System.out.println("Instantiate");
//            //os.writeBytes("First post");
//            //os.write("First post".getBytes());
//            //os.writeBytes("First post");
//            os.writeBytes(createStringBody());
//            //os.write(5);
//            System.out.println("Instantiate");
//            os.flush();
//            System.out.println("flush");
//            os.close();
//            System.out.println(connection.getResponseCode());
//           // writeIt(os);
//            System.out.println("close");
//
//
//            InputStream serverResponse = new BufferedInputStream(connection.getInputStream());
//            reader = new BufferedReader(new InputStreamReader(serverResponse));
//            System.out.println("Checkpoint6");
//            StringBuilder sb = new StringBuilder();
//            String line = "";
//
//            while((line = reader.readLine()) != null){
//                sb.append(line + "\n");
//                System.out.println("Checkpoint7");
//            }
//
//            text = sb.toString();
//            System.out.println("Checkpoint8");
//            //return "post worked";
//            return text;
//        }
//        finally {
//            if(os != null){
//                os.close();
//                reader.close();
//            }
//        }
//    }
//
//    public void writeIt(OutputStream stream) throws IOException{
//        String output = "First post";
//        stream.write(output.getBytes());
//        stream.flush();
//    }

    public String createStringBody(){
        String concatenated;
        concatenated = "Mode:\t" + MainActivity.PlayerModeSelected + "\n" + "Player1 Color: \t" + MainActivity.Player1Color +  "\n" + "Player2 Color: \t" + MainActivity.Player2Color;
        return concatenated;
    }

}