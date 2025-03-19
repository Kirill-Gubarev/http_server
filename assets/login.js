document.addEventListener("DOMContentLoaded", function () {
    const loginTab = document.getElementById("login-tab");
    const registerTab = document.getElementById("register-tab");
    const loginForm = document.getElementById("login-form");
    const registerForm = document.getElementById("register-form");

    
    loginTab.addEventListener("click", function () {
        loginForm.style.display = "block";
        registerForm.style.display = "none";
        loginTab.classList.add("active");
        registerTab.classList.remove("active");
    });

    registerTab.addEventListener("click", function () {
        loginForm.style.display = "none";
        registerForm.style.display = "block";
        registerTab.classList.add("active");
        loginTab.classList.remove("active");
    });

    
    loginForm.addEventListener("submit", function (event) {
        event.preventDefault();

        const login = document.getElementById("login").value;
        const password = document.getElementById("password").value;
        const action = "login";

        fetch("login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ action, login, password })
        })
        .then(response => response.json())
        .then(data => {
            if (data.login) {  
                localStorage.setItem("user", JSON.stringify(data));
                window.location.href = "index";
            } else {
                alert("Неверные данные!");
            }
        })
        .catch(error => {
            console.error("Ошибка:", error);
            alert("Произошла ошибка при подключении к серверу.");
        });
    });

    
    registerForm.addEventListener("submit", function (event) {
        event.preventDefault();

        const fullname = document.getElementById("reg-fullname").value;
        const email = document.getElementById("reg-email").value;
        const login = document.getElementById("reg-login").value;
        const password = document.getElementById("reg-password").value;
        const passwordConfirm = document.getElementById("reg-password-confirm").value;
        const action = "registration";

        if (password !== passwordConfirm) {
            alert("Пароли не совпадают!");
            return;
        }

        fetch("register", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ action, fullname, email, login, password })
        })
        .then(response => response.json())
        .then(data => {
            if (data.login) {  
                localStorage.setItem("user", JSON.stringify(data));
                window.location.href = "index";
            } else {
                alert("Ошибка регистрации: имя пользователя занято");
            }
        })
        .catch(error => {
            console.error("Ошибка:", error);
            alert("Произошла ошибка при подключении к серверу.");
        });
    });

});
