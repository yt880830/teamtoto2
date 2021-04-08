package syuppin;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dao.ItemtourokuDAO;
import tool.Action;
public class ItemTourokuAction extends Action{
//getIDでテーブルに入れるIDをとる  usernameもとる。
	public String execute(
			HttpServletRequest request, HttpServletResponse response
		) throws Exception {

		int id = Integer.parseInt(request.getParameter("id"));
		String username=request.getParameter("username");
		String rice=request.getParameter("price");                      //isEmptyに使うためだけのもの
		int price=Integer.parseInt(request.getParameter("price"));
		String title=request.getParameter("title");
		String introduction=request.getParameter("introduction");
		String genre=request.getParameter("genre");



    	if (rice.isEmpty()  || title.isEmpty()|| introduction.isEmpty() || genre.isEmpty() ) {
			return "/grasys/itemtouroku-error-empty.jsp";
		}

		ItemtourokuDAO dao=new ItemtourokuDAO();                //追加した分の要素分ここに追加
		if (dao.insert(id,username, price, title,introduction,genre)==false) {
			return "/grasys/itemtouroku-error-insert.jsp";
		}
		//ここにオプション追加daoの起動


		return "/grasys/itemtouroku-out.jsp";
	}


}
