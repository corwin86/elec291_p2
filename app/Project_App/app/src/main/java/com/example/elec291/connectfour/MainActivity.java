package com.example.elec291.connectfour;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.support.v7.app.AppCompatActivity;
import android.app.Activity;
import android.util.DisplayMetrics;
import android.view.Window;
import android.widget.Button;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.view.View;
import android.widget.Toast;
import android.view.View.OnClickListener;

public class MainActivity extends Activity{

    Button playButton;
    //static String PlayerModeSelected;
    static String Player1Color;
    static String Player2Color;


    static Drawable player1Color;
    static Drawable player2Color;

    static String urlToConnection = "http://192.168.43.82/";


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Empty Window
        super.onCreate(savedInstanceState);

        //Load XML File
        setContentView(R.layout.activity_main);

        //Change title of action bar
        setTitle("  ");



        //Initialization
        playButton = (Button) findViewById(R.id.playButton);
        playButton.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v) {
                //startActivity(new Intent(MainActivity.this, Pop.class));
                startActivity(new Intent(MainActivity.this, ChoosePlayerMode.class));
            }
        });
//        playButton.setOnClickListener(new View.OnClickListener(){
//
//
//
//            @Override
//            public void onClick(View v){
//                startActivity(new Intent(MainActivity.this, Pop.class));
//            }
//
//        });
        //playButton.setBackgroundColor(Color.parseColor("#FFF1D5D5"));
        //playButton.setColorFilter( 0xffff0000, Mode.MULTIPLY )    }

//        Paint paint = new Paint();
//        paint.setColor(Color.parseColor("#000000"));
//        Bitmap bg = Bitmap.createBitmap(480, 800, Bitmap.Config.ARGB_8888);
//        Canvas canvas = new Canvas(bg);
//        canvas.drawCircle(150,400,100,paint);
//        LinearLayout ll = (LinearLayout) findViewById(R.id.draw_area);
//        ll.setBackgroundDrawable(new BitmapDrawable(bg));



    }
    private void playButtonClick(){
        startActivity(new Intent("com.example.elec291.connect4.ChoosePlayerMode"));
    }



//    @Override
//    public void onClick(View v) {
//        switch ((v.getId())){
//            case R.id.playButton:
//                playButtonClick();
//                break;
//
//        }
//
//    }
}
 