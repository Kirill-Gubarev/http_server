document.addEventListener("DOMContentLoaded", function () {
    const userMenu = document.getElementById("user-menu");

    // Проверяем наличие пользователя в localStorage
    const user = JSON.parse(localStorage.getItem("user"));

    if (user) {
        // Обновляем меню для авторизованного пользователя
        userMenu.innerHTML = `
            <span id="balance">
                <i class="fas fa-wallet"></i> Баланс: ${user.balance} руб
            </span>
            <a href="cart" style="margin-right: 10px;"><i class="fas fa-shopping-cart"></i> Корзина</a>
            <a href="profile.html"><i class="fas fa-user"></i> ${user.login}</a>
        `;
    }
});

async function fetchProducts() {
    try {
        const response = await fetch(`fruits`);
        const products = await response.json();

        localStorage.setItem('products', JSON.stringify(products));

        const productList = document.getElementById('product-list');
        productList.innerHTML = ''; // Очистить список перед добавлением новых товаров

        products.forEach(product => {
            const productElement = document.createElement('div');
            productElement.classList.add('product');

            const productHTML = `
                <img src="${product.image}" alt="${product.name}">
                <p>${product.name} - ${product.price} руб/кг</p>
                <p>В наличии: ${product.quantity} кг</p> <!-- Отображаем количество на складе -->
                <button class="add-to-cart-btn" data-id="${product.id}" data-name="${product.name}" data-price="${product.price}" 
                        data-image="${product.image}" data-quantity="${product.quantity}">
                    В корзину
                </button>
            `;
            productElement.innerHTML = productHTML;
            productList.appendChild(productElement);
        });

        // Добавление обработчика событий для кнопок "В корзину"
        document.querySelectorAll('.add-to-cart-btn').forEach(button => {
            button.addEventListener('click', openCartModal);
        });
    } catch (error) {
        console.error('Ошибка при загрузке товаров:', error);
    }
}

function openCartModal(event) {
    const product = event.target;
    const productId = product.getAttribute('data-id');
    const productName = product.getAttribute('data-name');
    const productPrice = product.getAttribute('data-price');
    const productImage = product.getAttribute('data-image');
    const productquantity = parseInt(product.getAttribute('data-quantity'), 10); // Доступное количество на складе

    // Устанавливаем данные товара в модальном окне
    document.getElementById('product-name').textContent = productName;
    document.getElementById('product-price').textContent = productPrice;
    document.getElementById('product-image').src = productImage;
    document.getElementById('product-quantity').textContent = productquantity;

    // Устанавливаем ограничения на ввод количества
    const quantityInput = document.getElementById('quantity');
    quantityInput.value = 1; // Сбросить на 1 при открытии
    quantityInput.max = productquantity; // Устанавливаем максимальное значение для нового товара

    // Убираем предыдущий обработчик, если он был
    quantityInput.oninput = function () {
        // Оставляем только цифры в значении
        let value = this.value.replace(/[^0-9]/g, '');

        // Если введенное значение больше max, устанавливаем max
        if (parseInt(value, 10) > productquantity) {
            value = productquantity;
        }

        // Обновляем значение в поле ввода
        this.value = value;
    };


    // Отображаем модальное окно
    document.getElementById('cart-modal').style.display = 'block';

    // Добавление товара в корзину
    document.getElementById('add-to-cart').onclick = function() {
        const quantity = parseInt(quantityInput.value, 10);
        addToCart(productId, productName, productPrice, quantity);
        closeModal();
    };

    // Закрытие модального окна
    document.getElementById('cancel').onclick = closeModal;
    document.getElementById('close-modal').onclick = closeModal;
}


function closeModal() {
    document.getElementById('cart-modal').style.display = 'none';
}

function addToCart(productId, productName, productPrice, quantity) {
    let cart = JSON.parse(localStorage.getItem('cart')) || [];
    const product = {
        id: productId,
        name: productName,
        price: productPrice,
        quantity: quantity
    };
    cart.push(product);
    localStorage.setItem('cart', JSON.stringify(cart));

    // Отправляем данные на сервер
    sendCartData(productName, quantity);
}


async function sendCartData(productName, quantity) {
    // Извлекаем логин пользователя из localStorage
    const user = JSON.parse(localStorage.getItem('user'));

    if (!user) {
        console.error('Пользователь не найден');
        return;
    }

    const data = {
        action: 'add_cart',
        login: user.login,
        product: productName,
        quantity: quantity
    };

    fetch("cart", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data),  // Отправляем данные в формате JSON
    })
    .catch(error => {
        console.error("Ошибка:", error);
        alert("Произошла ошибка при подключении к серверу.");
    });
}

document.addEventListener('DOMContentLoaded', fetchProducts);
