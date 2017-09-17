function Execute()
    --CallAndPrint({Add,Sub,Mul})
    --Test:TestFunc()
    str = "Hello"
end

function Mul(value_1, value_2)
    return value_1 * value_2
end

function Add(value_1, value_2)
    return value_1 + value_2
end

function Sub(value_1, value_2)
    return value_1 - value_2
end

function Square(value)
    return value * value
end

function TestCall()
    pos = Point.new(10,10)
    pos.y = 20
    Print(pos)
    Print("x:"..pos.x)
    Print("y:"..pos.y)
    Print(pos:isZero())
end

function Update()
end

function main()
    font = Font.new(50)
    textureCat = Texture.new(Emoji.new("🐈"), TextureDesc.Mipped)
    Graphics.SetBackground(ColorF.new(0.8, 0.9, 1.0))
    
    while System.Update() do
        font("Hello, Siv3D!🐣"):drawAt(Window.Center(), Palette.Black)
        font(Cursor.Pos()):draw(20,400, ColorF.new(0.6))
        textureCat:resize(80):draw(540, 380)
		Circle
            :new(Cursor.Pos(), 60)
            :draw(MouseL:pressed() and Palette.Black or ColorF.new(1, 0, 0, 0.5));
    end
end

function f(arg)
    coroutine.yield(arg + 1)
    coroutine.yield(arg + 2)
    coroutine.yield(arg + 4)
    return 0
end