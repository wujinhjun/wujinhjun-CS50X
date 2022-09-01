import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userCash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]
    # print(userCash)
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as shares, operation FROM repository WHERE userID = ? GROUP BY symbol HAVING (SUM(shares));",
        session["user_id"]
    )

    # print(stocks)
    totalStock = 0
    for stock in stocks:
        quote = lookup(stock['symbol'])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = quote["price"] * stock["shares"]
        totalStock += stock["value"]
    totalValue = userCash + totalStock

    return render_template(
        "index.html", stocks=stocks, userCash=userCash, total=totalValue
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        price = lookup(symbol)

        try:
            shares = int(shares)
        except ValueError:
            return apology("must provide valid number", 400)

        # Ensure the input is valid
        if not symbol:
            print("no symbol")
            return apology("must provide username", 400)
        elif not price:
            print("no price")
            return apology("invalid symbol", 400)
        elif not shares:
            print("no shares")
            return apology("must provide shares", 400)
        elif not isinstance(shares, int) or shares <= 0:
            print("no int")
            return apology("shares must be a positive integer", 400)
        elif price is None:
            return apology("symbol must exist", 400)

        price = price["price"]
        # Query database by user id for cash
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Ensure the cash equal to finish this submit
        # print(price)
        totalPrice = shares * price

        if totalPrice > userCash:
            print("not enough")
            return apology("cannot afford the number of shares")
        else:
            date = datetime.datetime.now()
            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", totalPrice, session["user_id"])
            db.execute("INSERT INTO repository (userID, symbol, shares, price, date,operation) VALUES(?, ?, ?, ?, ?, ?)",
                session["user_id"], symbol, shares, price, date, "buy")

            flash("bought successful!")
            return redirect("/")
    else:
        return render_template("buy.html")
    # return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute(
        "SELECT * FROM repository WHERE userID = ?;",
        session["user_id"]
    )

    # print(stocks)
    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            print(len(rows))
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))
        # print(quote)
        if not quote:
            return apology("must provide correct symbol", 400)
        else:
            return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=quote["price"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # print("false")
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # print(f"{username} + {password} + {confirmation}")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username no exists
        if len(rows) != 0:
            print("1")
            return apology("this username already exists", 400)
        # Ensure username was submitted
        elif not username:
            print("2")
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not password:
            print("3")
            return apology("must provide password", 400)
        # Ensure password was confirmed
        elif not confirmation:
            print("4")
            return apology("must confirm password", 400)
        # Ensure password match confirmation
        elif not password == confirmation:
            print("4")
            return apology("twice password not match", 400)
        # else
        else:
            # generate hash
            hash = generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
            # Redirect user to home page
            return redirect("/")
    else:
        return render_template("register.html")
    # return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        if not symbol:
            return apology("must select symbol", 400)
        elif not shares:
            return apology("must provide shares", 400)

        stockShares = db.execute(
            "SELECT SUM(shares) as shares FROM repository WHERE userID = ? AND symbol = ?;",
            session["user_id"],
            symbol
        )[0]["shares"]

        if shares > stockShares:
            return apology("shares is more than the exist", 400)

        price = lookup(symbol)["price"]
        stockTotal = price * shares
        date = datetime.datetime.now()
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", stockTotal, session["user_id"])
        db.execute(
            "INSERT INTO repository (userID, symbol, shares, price, date, operation) VALUES (?, ?, ?, ?, ?, ?);",
            session["user_id"], symbol, -shares, price, date, "sell"
        )

        flash("Sold successful")
        return redirect("/")

        # return null
    else:
        stocks = db.execute(
            "SELECT symbol, SUM(shares) as shares, operation FROM repository WHERE userID = ? GROUP BY symbol HAVING (SUM(shares));",
            session["user_id"]
        )
        symbols = []
        for stock in stocks:
            symbols.append(stock["symbol"])
        return render_template("sell.html", symbols=symbols)
    # return apology("TODO")


@app.route("/personal", methods=["GET", "POST"])
@login_required
def personal():
    """change personal infomation"""
    if request.method == "POST":
        password = request.form.get("password")
        newPassword = request.form.get("newPassword")
        newConfirmation = request.form.get("newPasswordConfirmation")

        # Ensure the input is valid
        if not password:
            print("no password")
            return apology("must provide password", 400)
        elif not newPassword:
            print("no newPassword")
            return apology("must provide newPassword", 400)
        elif not newConfirmation:
            print("no newConfirmation")
            return apology("must provide newConfirmation", 400)

        passwordHash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["hash"]

        if not check_password_hash(passwordHash, password):
            print("false")
            return apology("must provide correct password", 400)
        elif not newPassword == newConfirmation:
            print("4")
            return apology("twice password not match", 400)
         # generate hash
        hash = generate_password_hash(password)
        # db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", stockTotal, session["user_id"])
        db.execute("UPDATE users  SET hash = ? WHERE id = ?", hash, session["user_id"])
        # Redirect user to home page
        session.clear()
        return redirect("/")

    else:
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
        print(rows["hash"])
        return render_template("personal.html")