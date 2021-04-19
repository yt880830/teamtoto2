package co.bbreak.sokusen._4._3;
import  java.io.BufferedReader;
import  java.io.FileNotFoundException;
import  java.io.IOException;
import  java.nio.charset.Charset;
import  java.nio.file.Files;
import  java.nio.file.Paths;
import  java.util.ArrayList;
import  java.util.Arrays;
import  java.util.List;

public class ReadCsv {

	public static void main(String[] args) {
		// TODO 自動生成されたメソッド・スタブ
	List<List<String>>ret=new ArrayList<List<String>>();
	//
	BufferedReader br = null;

	try {
		//
		br=Files.newBufferedReader(Paths.get("c:\\sookusen\\sample.csv"),Charset.forName("Window-31J"));
		//Cvsファイルから読み込んだ行データを格納するString
		String line="";

		while((line=br.readLine())!=null) {
		//Cvsファイルnoの１行を格納するリスト
		List<String>tmpList=new ArrayList<String>();
		String array[]=line.split(",");
		//
		tmpList=Arrays.asList(array);
		//
		System.out.println(tmpList);
		//
		ret.add(tmpList);
		}
	}catch(FileNotFoundException e) {
		e.printStackTrace();
	}catch(IOException e) {
		e.printStackTrace();
	}finally{
		try {
			if(br!=null) {
				br.close();
			}
	}catch(IOException e) {
		e.printStackTrace();
		}
	}
	}

}
