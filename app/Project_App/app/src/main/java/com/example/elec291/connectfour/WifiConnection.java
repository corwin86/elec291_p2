package com.example.elec291.connectfour;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

/**
 * Created by rohini on 29/03/16.
 */
public class WifiConnection {


    public static String POST(String url) throws IOException {

        try {
            URL connect_url = new URL(url);
            HttpURLConnection httpConnection = (HttpURLConnection)  connect_url.openConnection();

            //Posting Contect
            httpConnection.setRequestMethod("GET");
            httpConnection.setDoOutput(true);
            httpConnection.connect();
            //httpConnection.setDoInput(true);
            //httpConnection.setChunkedStreamingMode(0);

//            httpConnection.setRequestProperty("Content-Type", "application/json");
//            httpConnection.setRequestProperty("Accept", "application/json");
//            httpConnection.setRequestMethod("POST");

           //
           // OutputStream out = new BufferedOutputStream(httpConnection.getOutputStream());

            BufferedReader reader = new BufferedReader(new InputStreamReader(httpConnection.getInputStream()));
            String line;

            while((line = reader.readLine())!= null){
                System.out.println(line);
            }

            reader.close();
            httpConnection.disconnect();

            return line;

//            OutputStreamWriter outStream = new OutputStreamWriter(httpConnection.getOutputStream());
//            outStream.write("App is connected");
//            outStream.flush();
//            //writeStream(out);
//
//            InputStream in = new BufferedInputStream(httpConnection.getInputStream());
//            String input = readStream(in);
        }
            finally {
               // httpConnection.disconnect();
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

    public static void writeStream(OutputStream out) throws IOException {
        String output = "Hello World";
        out.write(output.getBytes());
        out.flush();

//        StringBuilder sb = new StringBuilder();
//        BufferedReader r = new BufferedReader(new OutputStreamReader(out),1000);
//        for (String line = r.readLine(); line != null; line =r.readLine()){
//            sb.append(line);
//        }
//        out.close();
//        return sb.toString();
    }


}
