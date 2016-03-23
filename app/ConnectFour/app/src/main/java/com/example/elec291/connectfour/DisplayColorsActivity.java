package com.example.elec291.connectfour;

import android.graphics.Color;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import org.w3c.dom.Text;

public class DisplayColorsActivity extends AppCompatActivity implements View.OnClickListener {

    Button buttonRed, buttonBlue, buttonGreen, buttonMagenta, buttonYellow, buttonCyan;
    TextView colorText2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_colors);
        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);

        int width = dm.widthPixels;
        int height = dm.heightPixels;

        getWindow().setLayout((int) (width * 0.8), (int) (height * 0.8));

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        //initialization
        buttonRed = (Button) findViewById(R.id.buttonRed);
        buttonBlue = (Button) findViewById(R.id.buttonBlue);
        buttonGreen = (Button) findViewById(R.id.buttonGreen);
        buttonMagenta = (Button) findViewById(R.id.buttonMagenta);
        buttonYellow = (Button) findViewById(R.id.buttonYellow);
        buttonCyan = (Button) findViewById(R.id.buttonCyan);

        buttonRed.setOnClickListener(this);
        buttonBlue.setOnClickListener(this);
        buttonGreen.setOnClickListener(this);
        buttonMagenta.setOnClickListener(this);
        buttonYellow.setOnClickListener(this);
        buttonCyan.setOnClickListener(this);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }

    private void colorButtonClick(Button pressedButton){

        //Change the color of the pressed button to Grey since it has already been picked
        pressedButton.setBackgroundColor(Color.parseColor("#FF888888"));
        colorText2 = (TextView) findViewById(R.id.colorText2);
        colorText2.setText("Player 1");

        RelativeLayout.LayoutParams p = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        p.addRule(RelativeLayout.BELOW,pressedButton.getId());
        colorText2.setLayoutParams(p);
        //RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        //params.addRule(RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE);
        //colorText2.setLayoutParams(params);




        //Change the text to tell user to choose a different color
                ((TextView) findViewById(R.id.textView2)).setText("Second Color");

    }


    @Override
    public void onClick(View v) {
        switch ((v.getId())){
            case R.id.buttonRed:
                colorButtonClick(buttonRed);
                break;
            case R.id.buttonBlue:
                colorButtonClick(buttonBlue);
                break;
            case R.id.buttonGreen:
                colorButtonClick(buttonGreen);
                break;
            case R.id.buttonCyan:
                colorButtonClick(buttonCyan);
                break;
            case R.id.buttonYellow:
                colorButtonClick(buttonYellow);
                break;
            case R.id.buttonMagenta:
                colorButtonClick(buttonMagenta);
                break;
        }
    }
}
