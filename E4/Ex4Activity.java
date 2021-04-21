package edu.ufl.cise.os.ex4;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class Ex4Activity extends Activity {

    private Button leftButton, rightButton;
    private EditText leftText, rightText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex4);

        rightButton = (Button) findViewById(R.id.rightButton);
        leftButton = (Button) findViewById(R.id.leftButton);
        rightText = (EditText) findViewById(R.id.rightText);
        leftText = (EditText) findViewById(R.id.leftText);
    }

    public void onLeftClick(View view)
    {
        // When the left button is clicked...
        rightText.setText(leftText.getText().toString());
    }

    public void onRightClick(View view)
    {
        // When the right button is clicked...
        leftText.setText(rightText.getText().toString());
    }
}
