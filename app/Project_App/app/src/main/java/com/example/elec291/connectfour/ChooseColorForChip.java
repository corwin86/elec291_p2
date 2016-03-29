package com.example.elec291.connectfour;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 28/03/16.
 */
public class ChooseColorForChip extends AppCompatActivity implements View.OnClickListener {

    static Button red, blue, green, yellow, cyan, magenta;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_choose_color_for_chip);

        //Shrink Size of pop-up down
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height * 0.8));

        red = (Button) findViewById(R.id.buttonRed);
        blue = (Button) findViewById(R.id.buttonBlue);
        green = (Button) findViewById(R.id.buttonGreen);
        yellow = (Button) findViewById(R.id.buttonYellow);
        cyan = (Button) findViewById(R.id.buttonCyan);
        magenta = (Button) findViewById(R.id.buttonMagenta);

        red.setOnClickListener(this);
        blue.setOnClickListener(this);
        green.setOnClickListener(this);
        yellow.setOnClickListener(this);
        cyan.setOnClickListener(this);
        magenta.setOnClickListener(this);

    }

    @Override
    public void onClick(View v){

    }
}
