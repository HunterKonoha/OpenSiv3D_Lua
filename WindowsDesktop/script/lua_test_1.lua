function Execute()
    --CallAndPrint({Add,Sub,Mul})
    --Test:TestFunc()
    str = "Hello"
    return str
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
    col = Color.new(10, 20, 30, 40)
    textureCat = Texture.new(Emoji.new("🐈"), TextureDesc.Mipped)
    Graphics.SetBackground(ColorF.new(0.8, 0.9, 1.0))
    
    while System.Update() do
        --font("Hello, Siv3D!🐣"):drawAt(Window.Center(), Palette.Black)
        font(Cursor.Pos()):draw(20,400, ColorF.new(0.6))
        font(string.format("(%d, %d, %d, %d)", col.r, col.g, col.b, col.a)):draw(20,340, ColorF.new(0.6))
        textureCat:resize(80):draw(540, 380)
        Print(true)
		Circle
            :new(Cursor.Pos(), 60)
            :draw(MouseL:pressed() and Palette.Black or ColorF.new(1, 0, 0, 0.5));
    end
end

function f(arg)
    coroutine.yield("1")
    coroutine.yield("1")
    coroutine.yield("1")
    return "0"
end

Position = {}

function Position.new(_x, _y)
    local ret = {x = _x, y = _y}
    if ret.x == nil then
        ret.x = 0
    end
    if ret.y == nil then
        ret.y = 0
    end
    return setmetatable(ret, {__index = Position})
end

function Position:__tostring()
    return "(" .. self.x ..", " .. self.y .. ")"
end

function Position:__add(obj)
self.x = self.x + 10
    return Position.new(self.x + obj.x, self.y + obj.y)
end

function Position:__sub(obj)
    return Position.new(self.x - obj.x, self.y - obj.y)
end

function Position:__mul(obj)
    return Position.new(self.x * obj.x, self.y * obj.y)
end

function Position:__div(obj)
    return Position.new(self.x / obj.x, self.y / obj.y)
end

function Position:__len()
    return math.sqrt(self.x ^ 2 + self.y ^ 2)
end

function Position:__eq(obj)
    return self.x == obj.x and self.y == obj.y
end

function Position:__lt(obj)
    return self.x < obj.x and self.y < obj.y
end

function Position:__le(obj)
    return self.x <= obj.x and self.y <= obj.y
end