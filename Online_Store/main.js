document.addEventListener('DOMContentLoaded', () => {
  const cartIcon      = document.getElementById('cart-icon');
  const cartPopup     = document.getElementById('cart-popup');
  const cartItemsList = document.getElementById('cart-items');
  const cartTotalDiv  = document.getElementById('cart-total');
  const checkoutBtn   = document.getElementById('checkout-btn');

  let cart = {};

  fetch('stockdata.json')
    .then(response => response.json())
    .then(data => {
      data.forEach(item => {
        const stockSpan = document.getElementById('stock-' + item.id);
        if (stockSpan) {
          stockSpan.textContent = item.stock;
        }
      });
    })
    .catch(error => console.error('Error loading stockdata.json:', error));

  cartIcon.addEventListener('click', () => {
    cartPopup.style.display = (cartPopup.style.display === 'block') ? 'none' : 'block';
  });

  document.addEventListener('click', (e) => {
    if (!cartIcon.contains(e.target) && !cartPopup.contains(e.target)) {
      cartPopup.style.display = 'none';
    }
  });

  function updateCartDisplay() {
    cartItemsList.innerHTML = '';
    let totalSum = 0;
    for (let id in cart) {
      const item = cart[id];
      const subTotal = item.price * item.quantity;

      const li = document.createElement('li');
      li.dataset.id = id;

      const textNode = document.createTextNode(`${item.name} x ${item.quantity} → ${subTotal} RON`);
      li.appendChild(textNode);

      const removeBtn = document.createElement('button');
      removeBtn.textContent = '×';
      removeBtn.title = 'Remove a product';
      removeBtn.classList.add('remove-from-cart');
      removeBtn.dataset.id = id;
      li.appendChild(removeBtn);

      cartItemsList.appendChild(li);
      totalSum += subTotal;
    }

    cartTotalDiv.textContent = `Total: ${totalSum} RON`;

    const allRemoveBtns = cartItemsList.querySelectorAll('.remove-from-cart');
    allRemoveBtns.forEach(btn => {
      btn.addEventListener('click', handleRemoveFromCart);
    });
  }

  function handleAddToCart(e) {
    const productDiv = e.currentTarget.closest('.product-item');
    const id         = productDiv.getAttribute('data-id');
    const name       = productDiv.getAttribute('data-name');
    const price      = parseFloat(productDiv.getAttribute('data-price'));
    const stockSpan  = document.getElementById("stock-" + id);

    if(parseInt(stockSpan.textContent) > 0 ){
      if (!cart[id]) {
        cart[id] = {
          name: name,
          price: price,
          quantity: 0
        };
      }

      cart[id].quantity += 1;

      let currentStock = parseInt(stockSpan.textContent, 10);
      currentStock -= 1;
      stockSpan.textContent = currentStock;

      updateCartDisplay();
    }
  }

  function handleRemoveFromCart(e) {
    e.stopPropagation();
    const id = e.currentTarget.dataset.id;
    const stockSpan = document.getElementById("stock-" + id);

    if (cart[id] && cart[id].quantity > 0) {
      cart[id].quantity -= 1;

      let currentStock = parseInt(stockSpan.textContent, 10);
      currentStock = Math.max(0, currentStock + 1);
      stockSpan.textContent = currentStock;

      if (cart[id].quantity === 0) {
        delete cart[id];
      }

      updateCartDisplay();
    }
  }

  function handleCheckout() {
    cart = {};
    updateCartDisplay();
    alert('Comanda a fost plasată cu succes!');
  }

  const addButtons = document.querySelectorAll('.add-to-cart');
  addButtons.forEach(btn => {
    btn.addEventListener('click', handleAddToCart);
  });

  checkoutBtn.addEventListener('click', handleCheckout);
});
