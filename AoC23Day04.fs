open System.IO

let ProcessCard (s:string) =
    let numbers = s.Split(":")[1]
    let temp = Array.map(fun (x:string) -> let split = x.Trim().Replace("  ", " ").Split(' ')
                                           Array.map System.Int32.Parse split |> Set.ofArray) (numbers.Trim().Split("|"))
    (temp[0], temp[1])

let ToWinners (winning, numbers) =
    Set.intersect winning numbers |> Set.count

let ToPoints winners =
    if winners = 0 then 0 else (int)(2.0 ** ((float)winners-1.0))

[<EntryPoint>]
let main argv =
    let data = File.ReadLines(argv[0]) |> Seq.map ProcessCard
    let count = Seq.length data
    let winners = Seq.map ToWinners data
    Seq.map ToPoints winners |> Seq.sum |> printfn("Part 1: %A")
    let cardValues = seq{ for i in 1 .. count -> 1} |> Seq.toArray
    printfn("Part 2: %A") <| Seq.fold(fun t (i, w) -> 
                                        for x in i+1 .. (min (count-1) (i + w)) do cardValues[x] <- cardValues[x]+cardValues[i]
                                        t + cardValues[i]) 0 (Seq.indexed winners)
    0