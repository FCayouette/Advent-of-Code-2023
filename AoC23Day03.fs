open System.IO

let IsDigit c = c >= '0' && c <= '9'
let IsSymbol c = c <> '.' && not (IsDigit c)
let ToNum c = (uint64)(c - '0')

let IndexString (s:string) = (s + ".").ToCharArray() |> Array.indexed

let FindSymbols data = 
    Seq.fold(fun r (x, y) -> let i = Array.filter(fun p -> IsSymbol(snd p)) y
                             Array.append r (Array.map(fun (y, c) -> (x, y, c)) i)) [||] data

let FindNumbers numbers (x, a) =
    let b = Array.fold(fun (result, (start, finish, value)) (y, c) ->
                match IsDigit c with | false -> if value > 0UL then (Array.append result [|(start, finish, value)|], (0,0,0UL)) else (result, (0,0,0UL))
                                     | true -> if value > 0UL then (result, (start, y, value*10UL + (ToNum c))) else (result, (y, y, ToNum c))) ([||], (0,0,0UL)) a
    let c = Array.map(fun (s, f, v) -> (x, s, f, v)) (fst b)
    Array.append numbers c

let PosOnly (x, y, _) = (x, y)
let Touches (nx, ns, ne, _) (x,y) =  x >= nx-1 && x <= nx+1 && y >= ns-1 && y <= ne+1
let IsPart number symbols = Array.exists(fun x -> Touches number x) symbols
let ToVal (_,_,_, n) = n
let GetGear numbers g =
    let touching = Array.filter(fun n -> Touches n g) numbers
    if Array.length touching < 2 then 0UL 
    else Array.map ToVal touching |> Array.reduce(*)

[<EntryPoint>]
let main argv = 
    let data = File.ReadLines(argv[0]) |> Seq.map(IndexString) |> Seq.indexed
    let numbers = Seq.fold FindNumbers [||] data
    let rawSymbols = FindSymbols data
    let gears = Array.filter(fun (_, _, c) -> c = '*') rawSymbols |> Array.map PosOnly
    let symbols = Array.map PosOnly rawSymbols

    Array.filter(fun n -> IsPart n symbols) numbers |> Array.fold(fun t x -> t + ToVal x) 0UL |> printfn("Part 1: %A")
    Array.fold(fun t s -> t + GetGear numbers s) 0UL gears |> printfn("Part 2: %A")
    0
