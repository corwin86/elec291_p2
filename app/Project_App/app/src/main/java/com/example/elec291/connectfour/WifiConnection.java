package com.example.elec291.connectfour;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeoutException;


/**
 * Created by rohini on 29/03/16.
 */
public class WifiConnection extends AppCompatActivity {

    private Context context;

    public WifiConnection(Context context){
        this.context = context;
    }


    private String stringToBePosted;


    public String doPOST(String toBeSent, String urlString) throws ExecutionException, InterruptedException {

        stringToBePosted = toBeSent;
        ConnectivityManager connmgr = (ConnectivityManager) context.getSystemService(context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connmgr.getActiveNetworkInfo();
        if(networkInfo != null && networkInfo.isConnected()){
            System.out.println("Network available");
            DownloadWebpageTask task = new DownloadWebpageTask();
            task.execute(urlString);
            String response = task.get();
            return response;

        }
        else{
            System.out.println("no network connection available");
        }
        return toBeSent;
    }



    public class DownloadWebpageTask extends AsyncTask<String, Void, String> {

        @Override
        protected String doInBackground(String...urls){

            //params comes from the execute() call
            boolean timedOut;
            String response = "";

            do {
                try {
                    timedOut = false;
                    response =  Upload(urls[0]);
                } catch (IOException e) {
                    return "unable to retrieve webpage";
                } catch (TimeoutException e) {
                    timedOut = true;
                }
            } while(timedOut);

            return response;
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
            URL connect_url = connect_url = new URL(myurl);
            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
            System.out.println("Checkpoint1");
            connection.setReadTimeout(15000);
            connection.setConnectTimeout(15000);
            connection.setRequestMethod("GET");
            connection.setDoInput(true);
            System.out.println("Checkpoint2");

            System.out.println("Checkpoint3");
            connection.connect();
            System.out.println("Checkpoint 4");
            System.out.println(connection.getResponseCode());
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

    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException {
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }

    private String Upload(String myurl) throws IOException, TimeoutException {
        DataOutputStream os = null;
        BufferedReader reader=  null;
        int len = 500;
        try{
            String text = "";
            URL connect_url = new URL(myurl);
            HttpURLConnection connection = (HttpURLConnection) connect_url.openConnection();
            System.out.println("Checkpoint1");
            connection.setReadTimeout(10000);
            connection.setConnectTimeout(60000);
            connection.setRequestMethod("POST");
            connection.setRequestProperty("Content-Type", "text/plain");
            connection.setDoOutput(true);
            connection.setDoInput(true);
            System.out.println("Checkpoint2");
            connection.connect();
            System.out.println("Checkpoint3");
            System.out.println("Checkpoint4");
            System.out.println("Checkpoint getOutputStream");
            os = new DataOutputStream(connection.getOutputStream());
            System.out.println("Instantiate");
            os.writeBytes(stringToBePosted);
            System.out.println("Instantiate");
            os.flush();
            System.out.println("flush");
            os.close();
            System.out.println(connection.getResponseCode());
            System.out.println("close");


            InputStream serverResponse = new BufferedInputStream(connection.getInputStream());
            reader = new BufferedReader(new InputStreamReader(serverResponse));
            System.out.println("Checkpoint6");
            StringBuilder sb = new StringBuilder();
            String line = "";

            while((line = reader.readLine()) != null){
                sb.append(line + "\n");
                System.out.println("Checkpoint7");
            }

            text = sb.toString();
            System.out.println("Checkpoint8");
            return text;
        }
        finally {
            if(os != null){
                os.close();
            }
            if(reader != null){
                reader.close();
            }
            else{
                throw new TimeoutException();
            }
        }
    }


}
