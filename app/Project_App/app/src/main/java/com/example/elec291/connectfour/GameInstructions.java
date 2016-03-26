package com.example.elec291.connectfour;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

/**
 * Created by rohini on 24/03/16.
 */
public class GameInstructions extends Activity implements View.OnClickListener{
    Button button2;
    final Activity gameActivity = this;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_game_instructions);

        button2 = (Button) findViewById(R.id.button2);
        //button2.setOnClickListener(this);
    }

    @Override
    public void onClick(View v){
     gameActivity.finishActivity(0);

    }
}
