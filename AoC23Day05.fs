open System.IO

let GroupData result (x:string) =
    if x.Length < 1 then 
         Array.append result [|[||]|]
    else let a = Array.append (Array.last result) [|x|]
         let c = Array.length result
         result[c-1] <- a
         result

let ToSeeds (x:string) = 
    let numbers = x.Split(": ")[1]
    Array.map System.Int64.Parse (numbers.Split(" "))

let ToSeedRanges seeds =
    fst (Array.fold(fun (ranges, t) x -> if t<> -1L then ((Array.append ranges [|(t, t+x-1L)|]), -1L)
                                         else (ranges, x)) ([||], -1L) seeds) |> Array.sort

let ProcessMapping (x: string array) =
    let _, values = Array.splitAt 1 x
    Array.map(fun (s:string) -> let a = Array.map System.Int64.Parse (s.Split(" "))
                                (a[1], a[1]+a[2]-1L, a[0])) values |> Array.sort
    
let Remap x map = 
    let work = Array.fold(fun (a, t) (s, e, d) -> if not t && a>=s && a<=e 
                                                    then (d+(a-s), true) 
                                                  else (a,t)) (x, false) map
    fst work

let RemapRange maps x =
    let (result, remain) = Array.fold(fun (t,(s, e)) (rs, re, d) -> 
        if e < rs || s > re || s = -1L then (t, (s,e))
        elif s < rs then 
            let straight = Array.append t [|(s, rs-1L)|]
            if e <= re then (Array.append straight [|(d, d + (e-rs))|], (-1L,-1L))
            else (Array.append straight [|(d, d+(re-rs))|], (re+1L, e))
        elif e <= re then (Array.append t [|(d+s-rs), (d+e-rs)|], (-1L, -1L))
        else (Array.append t [|(d+s-rs), (d+re-rs)|], (re+1L, e))) ([||], (x)) maps
    if fst remain = -1L then result
    else Array.append result [|remain|]

let ConsolidateRanges (r: (int64 * int64) array) = 
    Array.sortInPlace r
    let (start, toWork) = Array.splitAt 1 r
    Array.fold(fun t (ns, ne) -> 
        let (ps, pe) = Array.last t
        if pe + 1L < ns then Array.append t [|(ns, ne)|]
        else let index = Array.length t - 1
             t[index] <- (ps, ne)
             t) start toWork
    
let DoRemap ranges maps =
    Array.fold(fun t x -> Array.append t (RemapRange maps x)) [||] ranges |> ConsolidateRanges

[<EntryPoint>]
let main argv =
    let lines = File.ReadLines(argv[0]) |> Seq.toArray
    let lineCount = (Array.length lines) - 1
    let grouped = lines[2..lineCount] |> Array.fold GroupData [|[|lines[0]|]; [||]|] |> Array.splitAt 1
    let seeds = ToSeeds ((fst grouped)[0][0])
    let maps = Array.map ProcessMapping (snd grouped)
    let locations = Array.map (fun s -> Array.fold Remap s maps) seeds |> Array.sort
    printfn("Part 1: %A") locations[0]
    let LocationRanges = Array.fold DoRemap (ToSeedRanges seeds) maps
    printfn("Part 2: %A") <| fst LocationRanges[0]
    0