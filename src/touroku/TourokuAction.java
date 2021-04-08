package touroku;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dao.TourokuDAO;
import tool.Action;

public class TourokuAction extends Action {
	public String execute(
			HttpServletRequest request, HttpServletResponse response

		) throws Exception {

//		HttpSession session=request.getSession();
//		response.setCharacterEncoding("UTF-8");


		String name=request.getParameter("name");
		String username=request.getParameter("username");
		String password=request.getParameter("password");
		String address=request.getParameter("address");
		String mailaddress=request.getParameter("mailaddress");



    	if (name.isEmpty()  || username.isEmpty()|| password.isEmpty() || address.isEmpty() || mailaddress.isEmpty()) {
			return "/grasys/touroku-error-empty.jsp";
		}

		TourokuDAO dao=new TourokuDAO();                //追加した分の要素分ここに追加
		if (dao.insert( name, username,password,address,mailaddress)==false) {
			return "/grasys/touroku-error-insert.jsp";
		}

		return "/grasys/touroku-out.jsp";
	}


}
