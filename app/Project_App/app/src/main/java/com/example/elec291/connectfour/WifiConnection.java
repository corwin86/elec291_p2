package com.example.elec291.connectfour;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

/**
 * Created by rohini on 29/03/16.
 */
public class WifiConnection {


    public void POST(String url) throws IOException {
        try {
            URL connect_url = new URL(url);
            HttpURLConnection httpConnection = (HttpURLConnection)  connect_url.openConnection();
            InputStream in = new BufferedInputStream(httpConnection.getInputStream());
            readStream(in);
            finally {
                httpConnection.disconnect();
            }
        }
        catch(MalformedURLException e) {

        }

    }


}
