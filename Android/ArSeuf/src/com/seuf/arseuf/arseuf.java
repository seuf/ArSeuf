package com.seuf.arseuf;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import java.net.URI;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.content.Intent;
import java.io.IOException;
import java.net.URISyntaxException;
import android.widget.Toast;
import android.content.Context;

public class arseuf extends Activity
{

	private SharedPreferences pref;
	private Context context;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		pref = PreferenceManager.getDefaultSharedPreferences(this);
		context = getApplicationContext();


		setContentView(R.layout.main);
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {

		if (event.getRepeatCount() == 0) {
			switch (keyCode) {
				case KeyEvent.KEYCODE_MENU       : startActivity(new Intent(this, Preferences.class)); break; 
				case KeyEvent.KEYCODE_DPAD_CENTER: sendRequestToArduino("fwd"); break; // Cross = FORWARD
				case KeyEvent.KEYCODE_BUTTON_X   : sendRequestToArduino("back"); break; // square = BACKWARD
				case KeyEvent.KEYCODE_DPAD_LEFT  : sendRequestToArduino("tl"); break; // Left Dpad = turn left
				case KeyEvent.KEYCODE_DPAD_RIGHT : sendRequestToArduino("tr"); break; // Right Dpad = turn right
				case KeyEvent.KEYCODE_BUTTON_Y   : sendRequestToArduino("play"); break; // triangle = klaxon
			}
		}
		return true;
	}

	public boolean onKeyUp(int keyCode, KeyEvent event) {
		switch (keyCode) {
			case KeyEvent.KEYCODE_DPAD_CENTER: sendRequestToArduino("stop"); break;
			case KeyEvent.KEYCODE_BUTTON_X   : sendRequestToArduino("stop"); break;
			case KeyEvent.KEYCODE_DPAD_LEFT  : sendRequestToArduino("stopt"); break;
			case KeyEvent.KEYCODE_DPAD_RIGHT : sendRequestToArduino("stopt"); break;
		}
		return true;
	}

	public boolean sendRequestToArduino(String cmd) {

		HttpClient hc = new DefaultHttpClient();
		HttpClient client = new DefaultHttpClient();
		HttpGet request = new HttpGet();
		try {
	        	request.setURI(new URI( "http://" + pref.getString("arduino_ip", null) + "/" + cmd ));
		} catch (URISyntaxException e) {
			Toast.makeText(context, "URISyntaxException : "+e.getMessage(), Toast.LENGTH_SHORT).show();
		}
		try {
        	    	HttpResponse response = client.execute(request);
		} catch (IOException e) {
			Toast.makeText(context, "IOException : "+e.getMessage(), Toast.LENGTH_SHORT).show();
		}
		return true;
	}

}
