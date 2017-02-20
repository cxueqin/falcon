$(document).ready(function(){
    $("#login_btn").click(function(){
        var xmlhttp;
        var login = $("[name='login']").val();
        var passwd = $("[name='password']").val();
		if(window.XMLHttpRequest)
		{
			xmlhttp = new XMLHttpRequest();

		}else{
			xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
		}
		xmlhttp.onreadystatechange = function(){
			if(xmlhttp.readyState == 4 && xmlhttp.status == 200){
				var obj = JSON.parse(xmlhttp.responseText);
				if(obj.status === "success"){
                    checkCookie(login);
                    $("#loginForm").submit();
                }else{
                    alert("账号或密码错误");
                }
			}
		}
		xmlhttp.open("GET","/login.asp?login="+login+"&password="+passwd+"",true);			
		xmlhttp.send();
  });
  document.onkeydown = function(event) {    
            var target, code, tag;    
            if (!event) {    
                event = window.event; //针对ie浏览器    
                target = event.srcElement;    
                code = event.keyCode;    
                if (code == 13) {    
                    tag = target.tagName;    
                    if (tag == "TEXTAREA") { return true; }    
                    else { return false; }    
                }    
            }    
            else {    
                target = event.target; //针对遵循w3c标准的浏览器，如Firefox    
                code = event.keyCode;    
                if (code == 13) {    
                    tag = target.tagName;    
                    if (tag == "INPUT") { return false; }    
                    else { return true; }    
                }    
            }    
    };
});

function getCookie(c_name)
{
    if(document.cookie.length>0)
    {
        c_start=document.cookie.indexOf(c_name+"=");
        if(c_start!=-1)
        {
            c_start=c_start+c_name.length+1;
            c_end=document.cookie.indexOf(";",c_start);
            if(c_end==-1)
                c_end=document.cookie.length;
            return unescape(document.cookie.substring(c_start,c_end));
        }
    }
    return "";
}

function setCookie(c_name,value,expiredays)
{
    var exdate = new Date();
    exdate.setDate(exdate.getDate()+expiredays);
 
    document.cookie=c_name+"="+escape(value)+((expiredays==null)? "":";expires="+exdate.toGMTString());
 
}

function clearCookie(name) {
    setCookie(name, "", null);
}  

function checkCookie(c_name){
    userName = getCookie('userName');
    if(userName != null && userName != ""){
        alert('welcome again' + userName +"1");
    }else{
        setCookie('userName' , c_name, null);
    }
}