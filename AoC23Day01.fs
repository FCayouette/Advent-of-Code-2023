open System.IO

let Part1 (x:string) =
    let asCharArray = x.ToCharArray()
    let isNum c = c >'0' && c <='9'
    let toInt c = (int)(c-'0')
    (Array.find(isNum) asCharArray |> toInt) * 10 + (Array.findBack(isNum) asCharArray |> toInt) 

let Part2 (x:string)=
    let converted = x.Replace("one", "o1e").Replace("two", "t2").Replace("three", "t3e").Replace("four","4").Replace("five", "5e")
                        .Replace("six", "6").Replace("seven", "7n").Replace("eight", "8e").Replace("nine", "9")
    Part1 converted
    
[<EntryPoint>]
let main argv =
    let data = File.ReadLines(argv[0])
    Seq.map Part1 data |> Seq.sum |> printfn "Part 1: %A"
    Seq.map Part2 data |> Seq.sum |> printfn "Part 2: %A"
    0

