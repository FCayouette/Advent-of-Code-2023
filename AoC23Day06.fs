open System.IO

let ConvertLine (s:string) = 
    let mutable temp = s.Trim()
    while temp.Contains("  ") do
        temp <- temp.Replace("  ", " ")
    let nums = temp.Split(' ')
    Array.map System.UInt64.Parse nums

let NumberOfWins t d =
    seq {for i in 0UL .. t -> i*(t-i)} |> Seq.filter(fun x -> x > d) |> Seq.length

[<EntryPoint>]
let main argv =
    let input = File.ReadLines(argv[0]) |> Seq.toArray |> Array.map(fun (s:string) -> s.Split(": ")[1])
    let values = Array.map ConvertLine input
    Array.map2 NumberOfWins values[0] values[1] |> Array.reduce(*) |> printfn("Part 1: %A")
    let longInputs = Array.map(fun (s:string) -> System.UInt64.Parse(s.Replace(" ", ""))) input
    NumberOfWins longInputs[0] longInputs[1] |> printf("Part 2: %A")
    0